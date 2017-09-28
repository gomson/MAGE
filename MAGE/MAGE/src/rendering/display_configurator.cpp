//-----------------------------------------------------------------------------
// Engine Includes
//-----------------------------------------------------------------------------
#pragma region

#include "rendering\display_configurator.hpp"
#include "rendering\display_settings.hpp"
#include "rendering\rendering_utils.hpp"
#include "platform\windows_utils.hpp"
#include "ui\combo_box.hpp"
#include "file\file_utils.hpp"
#include "logging\error.hpp"

#pragma endregion

//-----------------------------------------------------------------------------
// Integrated + Dedicated GPU on notebooks  
//-----------------------------------------------------------------------------
#pragma region

/**
 NVIDIA Optimus enablement

 @pre NVIDIA Control Panel > Preferred graphics processor > "Auto-select"
 */
extern "C" {
	__declspec(dllexport) DWORD NvOptimusEnablement;
}

/**
 AMD "Optimus" enablement
 */
extern "C" {
	__declspec(dllexport) int AmdPowerXpressRequestHighPerformance;
}

#pragma endregion

//-----------------------------------------------------------------------------
// Engine Declarations and Definitions
//-----------------------------------------------------------------------------
namespace mage {
	
	INT_PTR CALLBACK DisplayConfigurator::DisplayDialogProcDelegate(
		HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {

		DisplayConfigurator * const display_configurator
			= GetDialogCaller< DisplayConfigurator >(
				hwndDlg, uMsg, wParam, lParam);
		
		return display_configurator->DisplayDialogProc(
				hwndDlg, uMsg, wParam, lParam);
	}

	const U32 DisplayConfigurator::s_nb_MSAA_samples[] = { 1, 2, 4, 8, 16 };

	DisplayConfigurator::DisplayConfigurator(
		DXGI_FORMAT pixel_format)
		: m_pixel_format(pixel_format), 
		m_display_configuration(),
		m_display_configuration_script(),
		m_adapter(), m_output(), 
		m_display_modes() {

		// Load the settings script.
		const bool file_exists 
			= FileExists(MAGE_DEFAULT_DISPLAY_SETTINGS_FILE);
		m_display_configuration_script = MakeUnique< VariableScript >(
			MAGE_DEFAULT_DISPLAY_SETTINGS_FILE, file_exists);

		// Initialize the adapter and output.
		InitializeAdapterAndOutput();
		// Initialize the display modes.
		InitializeDisplayModes();

	}

	DisplayConfigurator::DisplayConfigurator(
		ComPtr< IDXGIAdapter2 > adapter, ComPtr< IDXGIOutput2 > output,
		DXGI_FORMAT pixel_format)
		: m_pixel_format(pixel_format),
		
		m_display_configuration(), 
		m_display_configuration_script(),
		m_adapter(adapter), m_output(output), 
		m_display_modes() {

		// Load the settings script.
		const bool file_exists 
			= FileExists(MAGE_DEFAULT_DISPLAY_SETTINGS_FILE);
		m_display_configuration_script 
			= MakeUnique< VariableScript >(
				MAGE_DEFAULT_DISPLAY_SETTINGS_FILE, file_exists);

		// Initialize the display modes.
		InitializeDisplayModes();
	}

	DisplayConfigurator::DisplayConfigurator(
		DisplayConfigurator &&device_enumeration) = default;

	DisplayConfigurator::~DisplayConfigurator() = default;

	/**
	 Checks whether the given display mode needs to be rejected.

	 @param[in]		display_mode_desc
					A reference to a display mode descriptor.
	 @return		@c true if the given display mode needs to be rejected.
					@c false otherwise.
	 */
	inline bool RejectDisplayMode(const DXGI_MODE_DESC1 &display_mode_desc) noexcept {
		return display_mode_desc.Height < 480;
	}
	
	void DisplayConfigurator::InitializeAdapterAndOutput() {
		// Get the IDXGIFactory3.
		ComPtr< IDXGIFactory3 > factory;
		const HRESULT result_factory = CreateDXGIFactory1(
			__uuidof(IDXGIFactory3), (void**)factory.GetAddressOf());
		ThrowIfFailed(result_factory, 
			"IDXGIFactory3 creation failed: %08X.", result_factory);

		// Get the IDXGIAdapter1 and its primary IDXGIOutput.
		// The IDXGIAdapter represents a display subsystem (including one or more 
		// GPUs, DACs and video memory).
		ComPtr< IDXGIAdapter1 > adapter1;
		// The IDXGIOutput represents an adapter output (such as a monitor).
		ComPtr< IDXGIOutput > output;
		SIZE_T max_vram = 0;
		for (U32 i = 0u; factory->EnumAdapters1(i, adapter1.GetAddressOf()) 
			!= DXGI_ERROR_NOT_FOUND; ++i) {

			// Get the IDXGIAdapter2.
			ComPtr< IDXGIAdapter2 > adapter2;
			const HRESULT result_adapter2 = adapter1.As(&adapter2);
			ThrowIfFailed(result_adapter2, 
				"IDXGIAdapter2 creation failed: %08X.", result_adapter2);

			// Get the primary IDXGIOutput.
			const HRESULT result_output = adapter2->EnumOutputs(0, output.GetAddressOf());
			if (FAILED(result_output)) {
				continue;
			}
			// Get the IDXGIOutput2.
			ComPtr< IDXGIOutput2 > output2;
			const HRESULT result_output2 = output.As(&output2);
			ThrowIfFailed(result_output2, 
				"IDXGIOutput2 creation failed: %08X.", result_output2);

			// Get the DXGI_ADAPTER_DESC2.
			DXGI_ADAPTER_DESC2 desc;
			const HRESULT result_adapter_desc = adapter2->GetDesc2(&desc);
			ThrowIfFailed(result_adapter_desc, 
				"DXGI_ADAPTER_DESC2 retrieval failed: %08X.", result_adapter_desc);

			const SIZE_T vram = desc.DedicatedVideoMemory;
			if (vram <= max_vram) {
				continue;
			}

			m_adapter = adapter2;
			m_output  = output2;
			max_vram  = vram;
		}
	}
	
	void DisplayConfigurator::InitializeDisplayModes() {
		// Create the display modes linked list.
		m_display_modes = list< DXGI_MODE_DESC1 >();

		// Get the DXGI_MODE_DESCs. 
		const U32 flags = DXGI_ENUM_MODES_INTERLACED;
		U32 nb_display_modes;
		// Get the number of display modes that match the requested format 
		// and other input options.
		const HRESULT result1 
			= m_output->GetDisplayModeList1(
				m_pixel_format, flags, &nb_display_modes, nullptr);
		ThrowIfFailed(result1, 
			"Failed to get the number of display modes: %08X.", result1);
		UniquePtr< DXGI_MODE_DESC1[] >dxgi_mode_descs(
			MakeUnique< DXGI_MODE_DESC1[] >(nb_display_modes));
		// Get the display modes that match the requested format and other 
		// input options.
		const HRESULT result2 
			= m_output->GetDisplayModeList1(
				m_pixel_format, flags, &nb_display_modes, dxgi_mode_descs.get());
		ThrowIfFailed(result2, 
			"Failed to get the display modes: %08X.", result2);

		// Enumerate the DXGI_MODE_DESCs.
		for (U32 mode = 0u; mode < nb_display_modes; ++mode) {
			
			// Reject small display modes.
			if (RejectDisplayMode(dxgi_mode_descs.get()[mode])) {
				continue;
			}

			// Add the display mode to the list.
			m_display_modes.push_back(dxgi_mode_descs.get()[mode]);
		}
	}

	HRESULT DisplayConfigurator::Configure() const {
		// Creates a modal dialog box from a dialog box template resource.
		// 1. A handle to the module which contains the dialog box template. 
		//    If this parameter is nullptr, then the current executable is 
		//    used.
		// 2. The dialog box template.
		// 3. A handle to the window that owns the dialog box.
		// 4. A pointer to the dialog box procedure.
		// 5. The value to pass to the dialog box in the lParam parameter 
		//    of the WM_INITDIALOG message.
		const INT_PTR result_dialog 
			= DialogBoxParam(nullptr, MAKEINTRESOURCE(IDD_DISPLAY_SETTINGS),
			nullptr, DisplayDialogProcDelegate, reinterpret_cast< LPARAM >(this));
		
		return (result_dialog == IDOK) ? S_OK : E_FAIL;
	}
	
	/**
	 Converts the resolution of the given display format descriptor to a 
	 @c size_t.

	 @param[in]		desc
					A reference to the display format descriptor.
	 @return		A @c size_t value corresponding to the resolution of 
					the given display format descriptor.
	 */
	inline size_t ConvertResolution(const DXGI_MODE_DESC1 &desc) noexcept {
		return static_cast< size_t >(MAKELONG(desc.Width, desc.Height));
	}

	/**
	 Converts the refresh rate of the given display format descriptor to a 
	 @c size_t.

	 @param[in]		desc
					A reference to the display format descriptor.
	 @return		A @c size_t value corresponding to the refresh rate of 
					the given display format descriptor.
	 */
	inline size_t ConvertRefreshRate(const DXGI_MODE_DESC1 &desc) noexcept {
		return static_cast< size_t >(round(desc.RefreshRate.Numerator 
			/ static_cast< F32 >(desc.RefreshRate.Denominator)));
	}

	INT_PTR DisplayConfigurator::DisplayDialogProc(
		HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
		UNUSED(lParam);
		wchar_t buffer[16];

		// color depth (format) affects resolution affects refresh rate.

		switch (uMsg) {
		
		case WM_INITDIALOG: {
			// Sent to the dialog box procedure immediately before a dialog box is 
			// displayed. Dialog box procedures typically use this message to 
			// initialize controls and carry out any other initialization tasks 
			// that affect the appearance of the dialog box.

			// Display the adapter details.
			DXGI_ADAPTER_DESC2 desc;
			m_adapter->GetDesc2(&desc);
			Edit_SetText(GetDlgItem(hwndDlg, IDC_DISPLAY_ADAPTER), desc.Description);

			if (m_display_configuration_script->IsEmpty()) {
				m_display_configuration_script->AddVariable(
					VariableType::Bool, "windowed",   true);
				m_display_configuration_script->AddVariable(
					VariableType::Bool, "vsync",      false);
				m_display_configuration_script->AddVariable(
					VariableType::Int,  "resolution", 0);
				m_display_configuration_script->AddVariable(
					VariableType::Int, "msaa", 0);
				m_display_configuration_script->AddVariable(
					VariableType::Int,  "refresh",	  0);
			}
			
			// Load the windowed state.
			const bool windowed 
				= *m_display_configuration_script->GetValueOfVariable< bool >("windowed");
			// Change the check state of a button control.
			// 1. A handle to the dialog box that contains the button.
			// 2. The identifier of the button to modify.
			// 3. The check state of the button.
			CheckDlgButton(hwndDlg, IDC_WINDOWED, windowed);
			CheckDlgButton(hwndDlg, IDC_FULLSCREEN, !windowed);

			// Load the vsync state.
			const bool vsync 
				= *m_display_configuration_script->GetValueOfVariable< bool >("vsync");
			// Change the check state of a button control.
			// 1. A handle to the dialog box that contains the button.
			// 2. The identifier of the button to modify.
			// 3. The check state of the button.
			CheckDlgButton(hwndDlg, IDC_VSYNC, vsync);
			
			// Fill in the display formats combo box.
			
			// Remove all items from the list box and edit control of a combo box.
			ComboBox_ResetContent(GetDlgItem(hwndDlg, IDC_MSAA));
			for (size_t i = 0; i < _countof(s_nb_MSAA_samples); ++i) {
				swprintf_s(buffer, _countof(buffer), L"x%u", s_nb_MSAA_samples[i]);
				ComboBoxAddValue(hwndDlg, IDC_MSAA, 
					static_cast< size_t >(s_nb_MSAA_samples[i]), buffer);
			}
			const int msaa_index 
				= *m_display_configuration_script->GetValueOfVariable< int >("msaa");
			ComboBoxSelect(hwndDlg, IDC_MSAA, msaa_index);
			
			// Fill in the resolutions combo box associated with the current format.
			
			// Remove all items from the list box and edit control of a combo box.
			ComboBox_ResetContent(GetDlgItem(hwndDlg, IDC_RESOLUTION));
			for (const auto &mode : m_display_modes) {
				swprintf_s(buffer, _countof(buffer), L"%u x %u", mode.Width, mode.Height);
					
				if (!ComboBoxContains(hwndDlg, IDC_RESOLUTION, buffer)) {
					const size_t resolution = ConvertResolution(mode);
					ComboBoxAddValue(hwndDlg, IDC_RESOLUTION, resolution, buffer);
				}
			}
			const int resolution_index 
				= *m_display_configuration_script->GetValueOfVariable< int >("resolution");
			ComboBoxSelect(hwndDlg, IDC_RESOLUTION, resolution_index);

			const size_t selected_resolution = ComboBoxSelectedValue(hwndDlg, IDC_RESOLUTION);
			
			// Fill in the refresh rates combo box associated with the current resolution.
			
			// Remove all items from the list box and edit control of a combo box.
			ComboBox_ResetContent(GetDlgItem(hwndDlg, IDC_REFRESH_RATE));
			for (const auto &mode : m_display_modes) {
				const size_t resolution = ConvertResolution(mode);
				if (selected_resolution == resolution) {
					
					const size_t refresh_rate = ConvertRefreshRate(mode);
					swprintf_s(buffer, _countof(buffer), L"%u Hz", 
						static_cast< unsigned int >(refresh_rate));
		
					if (!ComboBoxContains(hwndDlg, IDC_REFRESH_RATE, buffer)) {
						ComboBoxAddValue(hwndDlg, IDC_REFRESH_RATE, refresh_rate, buffer);
					}
				}
			}
			const int refresh_rate_index 
				= *m_display_configuration_script->GetValueOfVariable< int >("refresh");
			ComboBoxSelect(hwndDlg, IDC_REFRESH_RATE, refresh_rate_index);

			return TRUE;
		}
		
		case WM_COMMAND: {
			// Sent when the user selects a command item from a menu, when a 
			// control sends a notification message to its parent window.
			
			switch (LOWORD(wParam)) {
			
			case IDOK: {
				
				// Store the details of the selected display mode.
				const size_t selected_resolution 
					= ComboBoxSelectedValue(hwndDlg, IDC_RESOLUTION);
				const size_t selected_msaa 
					= ComboBoxSelectedValue(hwndDlg, IDC_MSAA);
				const size_t selected_refresh_rate 
					= ComboBoxSelectedValue(hwndDlg, IDC_REFRESH_RATE);
				const DXGI_MODE_DESC1 *selected_diplay_mode 
					= nullptr;
				for (const auto &display_mode : m_display_modes) {
					
					const size_t resolution = ConvertResolution(display_mode);
					if (selected_resolution != resolution) {
						continue;
					}

					const size_t refresh_rate = ConvertRefreshRate(display_mode);
					if (selected_refresh_rate != refresh_rate) {
						continue;
					}

					selected_diplay_mode = &display_mode;
					break;
				}
				if (!selected_diplay_mode) {
					Error("Selected display mode retrieval failed.");

					// Close the hwndDlg.
					EndDialog(hwndDlg, IDCANCEL);

					return TRUE;
				}
				
				const bool windowed = IsDlgButtonChecked(hwndDlg, IDC_WINDOWED) 
									? true : false;
				const bool vsync	= IsDlgButtonChecked(hwndDlg, IDC_VSYNC)    
									? true : false;

				// Store all the settings to the display configuration.
				m_display_configuration = MakeUnique< DisplayConfiguration >(
					m_adapter, m_output, *selected_diplay_mode, windowed, vsync, 
					static_cast< U32 >(selected_msaa));

				// Get the selected index from each combo box.
				const int resolution_index
					= ComboBox_GetCurSel(GetDlgItem(hwndDlg, IDC_RESOLUTION));
				const int msaa_index 
					= ComboBox_GetCurSel(GetDlgItem(hwndDlg, IDC_MSAA));
				const int refresh_rate_index 
					= ComboBox_GetCurSel(GetDlgItem(hwndDlg, IDC_REFRESH_RATE));
				
				// Store all the settings to the display configuration script.
				m_display_configuration_script->SetValueOfVariable(
												"windowed",	windowed);
				m_display_configuration_script->SetValueOfVariable(
												"vsync", vsync);
				m_display_configuration_script->SetValueOfVariable(
												"resolution", resolution_index);
				m_display_configuration_script->SetValueOfVariable(
												"msaa", msaa_index);
				m_display_configuration_script->SetValueOfVariable(
												"refresh", refresh_rate_index);
				// Save all the settings in the display configuration script.
				m_display_configuration_script->ExportScript();

				// Close the hwndDlg.
				EndDialog(hwndDlg, IDOK);

				return TRUE;
			}
			
			case IDCANCEL: {
				// Close the hwndDlg.
				EndDialog(hwndDlg, IDCANCEL);

				return TRUE;
			}
			
			case IDC_RESOLUTION: {
				
				if (CBN_SELCHANGE == HIWORD(wParam)) {
					
					const size_t selected_resolution 
						= ComboBoxSelectedValue(hwndDlg, IDC_RESOLUTION);
					const size_t selected_refresh_rate 
						= ComboBoxSelectedValue(hwndDlg, IDC_REFRESH_RATE);

					// Update the refresh rate combo box.

					// Remove all items from the list box and edit control of a 
					// combo box.
					ComboBox_ResetContent(GetDlgItem(hwndDlg, IDC_REFRESH_RATE));
					for (const auto &mode : m_display_modes) {
						
						const size_t resolution = ConvertResolution(mode);
						
						if (selected_resolution == resolution) {
							
							const size_t refresh_rate = ConvertRefreshRate(mode);
							swprintf_s(buffer, _countof(buffer), L"%u Hz",
								static_cast< unsigned int >(refresh_rate));
							
							if (!ComboBoxContains(hwndDlg, IDC_REFRESH_RATE, buffer)) {
								ComboBoxAddValue(
									hwndDlg, IDC_REFRESH_RATE, refresh_rate, buffer);
								
								if (selected_refresh_rate == refresh_rate) {
									ComboBoxSelectValue(
										hwndDlg, IDC_REFRESH_RATE, selected_refresh_rate);
								}
							}
						}
					}

					if (ComboBoxSelected(hwndDlg, IDC_REFRESH_RATE) == nullptr) {
						ComboBoxSelect(hwndDlg, IDC_REFRESH_RATE, 0);
					}
				}

				return TRUE;
			}
			}
		}
		}
		
		return FALSE;
	}
}