//-----------------------------------------------------------------------------
// Engine Includes
//-----------------------------------------------------------------------------
#pragma region

#include "scripting\behavior_script.hpp"

#pragma endregion

//-----------------------------------------------------------------------------
// Engine Definitions
//-----------------------------------------------------------------------------
namespace mage {

	BehaviorScript::BehaviorScript() 
		: m_active(true), m_terminated(false) {}

	BehaviorScript::BehaviorScript(const BehaviorScript &script) = default;

	BehaviorScript::BehaviorScript(BehaviorScript &&script) = default;

	BehaviorScript::~BehaviorScript() = default;

	void BehaviorScript::FixedUpdate() {}

	void BehaviorScript::Update([[maybe_unused]] F64 delta_time) {}

	void BehaviorScript::SetActive(bool active) noexcept {
		if (m_terminated || (m_active == active)) {
			return;
		}

		m_active = active;

		OnActiveChange();
	}

	void BehaviorScript::OnActiveChange() noexcept {}

	void BehaviorScript::Terminate() noexcept {
		if (m_terminated) {
			return;
		}

		m_terminated = true;
		m_active     = false;

		OnActiveChange();
	}
}