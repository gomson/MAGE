//-----------------------------------------------------------------------------
// Engine Includes
//-----------------------------------------------------------------------------
#pragma region

#include "light\spot_light.hpp"

#pragma endregion

//-----------------------------------------------------------------------------
// Engine Definitions
//-----------------------------------------------------------------------------
namespace mage {

	SpotLight::SpotLight(const RGBSpectrum &intensity)
		: Light(intensity),
		m_distance_falloff_start(0.0f), m_distance_falloff_end(1.0f),
		m_cos_penumbra(0.96592583f), m_cos_umbra(0.86602540f),
		m_exponent_property(1.0f) {}

	SharedPtr< Light > SpotLight::CloneImplementation() const {
		return SharedPtr< SpotLight >(new SpotLight(*this));
	}
}