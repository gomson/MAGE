//-----------------------------------------------------------------------------
// Engine Includes
//-----------------------------------------------------------------------------
#pragma region

#include "core\engine.hpp"
#include "core\version.hpp"
#include "logging\error.hpp"
#include "logging\exception.hpp"
#include "logging\logging.hpp"
#include "rendering\device_enumeration.hpp"

#pragma endregion

//-----------------------------------------------------------------------------
// Engine Definitions
//-----------------------------------------------------------------------------
namespace mage {

	//-------------------------------------------------------------------------
	// Engine
	//-------------------------------------------------------------------------
	
	Engine *Engine::s_engine = nullptr;

	Engine::Engine(const EngineSetup &setup) 
		: Loadable(), m_device_enumeration(),
		m_main_window(), m_deactive(false), 
		m_renderer(), m_mode_switch(false),
		m_input_manager(), m_resource_manager(),
		m_scene(), m_timer(MakeUnique< Timer >()),
		m_engine_stats(MakeUnique< EngineStatistics >()) {

		s_engine = this;

		// Initialize the systems of this engine.
		InitializeSystems(setup);

		if (!IsLoaded()) {
			return;
		}

		// Initialize the first scene.
		SetScene(setup.CreateScene());
	}

	Engine::Engine(Engine &&engine) = default;

	Engine::~Engine() {

		// Uninitialize the COM.
		CoUninitialize();

		s_engine = nullptr;
	}

	void Engine::InitializeSystems(const EngineSetup &setup) {

		// Initialize a console.
		InitializeConsole();
		PrintConsoleHeader();

		// Enumerate the devices.
		m_device_enumeration = MakeUnique< DeviceEnumeration >();
		const HRESULT result_enumerate = m_device_enumeration->Enumerate();
		if (FAILED(result_enumerate)) {
			Error("Device enumeration setup failed: %ld", result_enumerate);
			return;
		}

		const LONG width  = static_cast< LONG >(m_device_enumeration->GetDisplayMode()->Width);
		const LONG height = static_cast< LONG >(m_device_enumeration->GetDisplayMode()->Height);
		
		// Initialize the window System.
		m_main_window      = MakeUnique< MainWindow >(setup.GetApplicationHinstance(), setup.GetApplicationName(), width, height);
		// Initialize the rendering system.
		m_renderer         = MakeUnique< Renderer >(m_main_window->GetHandle());
		// Initialize the input system.
		m_input_manager    = MakeUnique< InputManager >(m_main_window->GetHandle());
		// Initialize the resource system.
		m_resource_manager = MakeUnique< ResourceManager >();

		// Initializes the COM library for use by the calling thread 
		// and sets the thread's concurrency model to multithreaded concurrency.
		CoInitializeEx(nullptr, COINIT_MULTITHREADED);

		SetLoaded();
	}

	void Engine::SetDeactiveFlag(bool deactive) {
		m_deactive = deactive;
		
		if (m_deactive) {
			m_timer->Stop();
		}
		else {
			m_timer->Resume();
		}
	}

	void Engine::SetScene(SharedPtr< Scene > scene) {
		m_timer->Stop();
		
		if (m_scene) {
			m_scene->Uninitialize();
		}

		m_scene = scene;
		
		if (m_scene) {
			m_scene->Initialize();
		}
		
		m_timer->Restart();
	}

	int Engine::Run(int nCmdShow) {
		if (!IsLoaded()) {
			Error("Game loop can not start because the engine is not loaded.");
			return 0;
		}

		m_main_window->Show(nCmdShow);

		// Handle startup in fullscreen mode.
		if (m_device_enumeration->IsFullScreen()) {
			m_renderer->SwitchMode(true);
		}

		m_timer->Restart();

		// Enter the message loop.
		MSG msg;
		SecureZeroMemory(&msg, sizeof(msg));
		while (msg.message != WM_QUIT) {
			
			// Retrieves messages for any window that belongs to the current thread
			// without performing range filtering. Furthermore messages are removed
			// after processing.
			if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
				// Translates virtual-key messages into character messages.
				TranslateMessage(&msg);
				// Dispatches a message to a window procedure.
				DispatchMessage(&msg);
				continue;
			}

			if (m_deactive || !m_scene) {
				continue;
			}

			// Update the input manager.
			m_input_manager->Update();
			// Handle forced exit.
			if (m_input_manager->GetKeyboard()->GetKeyPress(DIK_F1)) {
				PostQuitMessage(0);
				continue;
			}

			// Handle switch between full screen and windowed mode.
			const bool lost_mode = m_renderer->LostMode();
			if (m_mode_switch || lost_mode) {
				m_renderer->SwitchMode(!lost_mode);
				m_mode_switch = false;
				continue;
			}

			// Calculate the elapsed time.
			const double delta_time = m_timer->GetDeltaTime();

			// Update the current scene.
			m_scene->Update(delta_time);
			if (!m_scene) {
				PostQuitMessage(0);
				continue;
			}
				
			// Render the current scene.
			m_engine_stats->PrepareRendering();
			m_renderer->BeginFrame();
			m_renderer->PrepareRendering3D();
			m_scene->Render3D();
			//m_scene->RenderBoundingBoxes();
			m_renderer->PrepareRendering2D();
			m_scene->Render2D();
			m_renderer->EndFrame();
		}

		return static_cast< int >(msg.wParam);
	}
}
