//-----------------------------------------------------------------------------
// Engine Includes
//-----------------------------------------------------------------------------
#pragma region

#include "input\input_manager.hpp"
#include "utils\logging\error.hpp"
#include "utils\exception\exception.hpp"

#pragma endregion

//-----------------------------------------------------------------------------
// Engine Definitions
//-----------------------------------------------------------------------------
namespace mage {

	const Keyboard *Keyboard::Get() noexcept {
		Assert(InputManager::Get());

		return InputManager::Get()->GetKeyboard();
	}

	Keyboard::Keyboard(HWND hwindow, IDirectInput8 *di) 
		: m_hwindow(hwindow), m_di(di), m_keyboard(),
		m_press_stamp(0), m_key_state{}, m_key_press_stamp{} {

		Assert(m_hwindow);
		Assert(m_di);

		InitializeKeyboard();
	}

	Keyboard::Keyboard(Keyboard &&keyboard) = default;

	Keyboard::~Keyboard() = default;

	void Keyboard::InitializeKeyboard() {
		// Create and initialize an instance of a device based on a given 
		// globally unique identifier (GUID), and obtain an IDirectInputDevice8 
		// Interface interface. 
		// 1. Reference to the GUID for the desired input device.
		// 2. Address of a variable to receive the IDirectInputDevice8 
		//    Interface interface pointer if successful.
		// 3. Pointer to the address of the controlling object's IUnknown 
		//    interface for COM aggregation, or nullptr if the interface is not 
		//    aggregated.
		const HRESULT result_keyboard_create = m_di->CreateDevice(
			GUID_SysKeyboard, m_keyboard.ReleaseAndGetAddressOf(), nullptr);
		ThrowIfFailed(result_keyboard_create, 
			"Keyboard device creation failed: %08X.", result_keyboard_create);
		
		// Set the data format for the DirectInput device. 
		const HRESULT result_keyboard_format = 
			m_keyboard->SetDataFormat(&c_dfDIKeyboard);
		ThrowIfFailed(result_keyboard_format, 
			"Setting data format for keyboard device failed: %08X.", 
			result_keyboard_format);

		// Establish the cooperative level for this instance of the device. 
		// The cooperative level determines how this instance of the device 
		// interacts with other instances of the device and the rest of the 
		// system. 
		const HRESULT result_keyboard_cooperative = 
			m_keyboard->SetCooperativeLevel(m_hwindow, 
				DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
		ThrowIfFailed(result_keyboard_cooperative, 
			"Setting cooperation level for keyboard device failed: %08X.", 
			result_keyboard_cooperative);

		// Obtain access to the input device. 
		m_keyboard->Acquire();
	}

	bool Keyboard::GetKeyPress(unsigned char key, 
		bool ignore_press_stamp) const {
		
		if ((m_key_state[key] & 0x80) == false) {
			return false;
		}

		const bool pressed = (!ignore_press_stamp 
			&& (m_key_press_stamp[key] == m_press_stamp - 1)) 
			? false : true;

		m_key_press_stamp[key] = m_press_stamp;

		return pressed;
	}

	void Keyboard::Update() {
		// Poll the keyboard until it succeeds or returns an unknown error.
		while (true) {
			// Retrieves data from polled objects on a DirectInput device.
			m_keyboard->Poll();

			// Retrieves immediate data from the device. 
			// 1. Size of the buffer in bytes.
			// 2. Address of a structure that receives the current state of 
			//    the device. (format determined by SetDataFormat)
			const HRESULT result_keyboard_state = 
				m_keyboard->GetDeviceState(256, (LPVOID)&m_key_state);
			if (SUCCEEDED(result_keyboard_state)) {
				break;
			}
			if (result_keyboard_state != DIERR_INPUTLOST 
				&& result_keyboard_state != DIERR_NOTACQUIRED) {
				return;
			}
			// Reacquire the device if the focus was lost.
			if (FAILED(m_keyboard->Acquire())) {
				return;
			}
		}

		// Increment the press stamp.
		++m_press_stamp;
	}
}