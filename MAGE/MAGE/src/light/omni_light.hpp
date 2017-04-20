//-----------------------------------------------------------------------------
// Engine Includes
//-----------------------------------------------------------------------------
#pragma region

#include "light\light.hpp"

#pragma endregion

//-----------------------------------------------------------------------------
// Engine Declarations and Definitions
//-----------------------------------------------------------------------------
namespace mage {

	class OmniLight : public Light {

	public:

		//---------------------------------------------------------------------
		// Constructors and Destructors
		//---------------------------------------------------------------------

		explicit OmniLight(const string name, const RGBSpectrum &intensity,
			float distance_falloff_start = 0.0f, 
			float distance_falloff_end = 10.0f)
			: Light(name, intensity), 
			m_distance_falloff_start(distance_falloff_start),
			m_distance_falloff_end(distance_falloff_end) {}
		OmniLight(const OmniLight &light) = default;
		OmniLight(OmniLight &&light) = default;
		virtual ~OmniLight() = default;

		//---------------------------------------------------------------------
		// Assignment Operators
		//---------------------------------------------------------------------	

		OmniLight &operator=(const OmniLight &light) = default;
		OmniLight &operator=(OmniLight &&light) = default;

		//---------------------------------------------------------------------
		// Member Methods
		//---------------------------------------------------------------------

		virtual OmniLight *Clone() const {
			return new OmniLight(*this);
		}
		
		const XMVECTOR GetWorldLightPosition() const {
			return GetTransform()->GetWorldEye();
		}
		
		float GetStartDistanceFalloff() const {
			return m_distance_falloff_start;
		}
		void SetStartDistanceFalloff(float distance_falloff_start) {
			m_distance_falloff_start = distance_falloff_start;
		}
		float GetEndDistanceFalloff() const {
			return m_distance_falloff_end;
		}
		void SetEndDistanceFalloff(float distance_falloff_end) {
			m_distance_falloff_end = distance_falloff_end;
		}
		void SetDistanceFalloff(float distance_falloff_start, float distance_falloff_end) {
			SetStartDistanceFalloff(distance_falloff_start);
			SetEndDistanceFalloff(distance_falloff_end);
		}

	private:

		float m_distance_falloff_start;
		float m_distance_falloff_end;
	};
}