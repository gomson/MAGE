#pragma once

//-----------------------------------------------------------------------------
// Engine Includes
//-----------------------------------------------------------------------------
#pragma region

#include "scripting\behavior_script.hpp"
#include "utils\collection\collection.hpp"
#include "camera\camera_settings.hpp"

#pragma endregion

//-----------------------------------------------------------------------------
// Engine Declarations and Definitions
//-----------------------------------------------------------------------------
namespace mage::script {

	class RenderModeScript final : public BehaviorScript {

	public:

		//---------------------------------------------------------------------
		// Constructors and Destructors
		//---------------------------------------------------------------------

		RenderModeScript(CameraSettings *settings);
		RenderModeScript(const RenderModeScript &script) = delete;
		RenderModeScript(RenderModeScript &&script);
		virtual ~RenderModeScript();

		//---------------------------------------------------------------------
		// Assignment Operators
		//---------------------------------------------------------------------

		RenderModeScript &operator=(const RenderModeScript &script) = delete;
		RenderModeScript &operator=(RenderModeScript &&script) = delete;

		//---------------------------------------------------------------------
		// Member Methods
		//---------------------------------------------------------------------

		virtual void Update([[maybe_unused]] F64 delta_time) override;

	private:

		//---------------------------------------------------------------------
		// Member Variables
		//---------------------------------------------------------------------

		CameraSettings * const m_settings;
		vector< RenderMode > m_render_modes;
		vector< const char * > m_render_mode_names;
		int m_index;
		bool m_wireframe;
		bool m_aabb;
	};
}