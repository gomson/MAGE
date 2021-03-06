#pragma once

//-----------------------------------------------------------------------------
// Engine Includes
//-----------------------------------------------------------------------------
#pragma region

#include "light\light.hpp"
#include "camera\perspective_camera.hpp"

#pragma endregion

//-----------------------------------------------------------------------------
// Engine Declarations and Definitions
//-----------------------------------------------------------------------------
namespace mage {

	/**
	 A class of omni lights.
	 */
	class OmniLight final : public Light {

	public:

		//---------------------------------------------------------------------
		// Constructors and Destructors
		//---------------------------------------------------------------------

		/**
		 Constructs an omni light.
		 */
		OmniLight();
		
		/**
		 Constructs an omni light from the given omni light.

		 @param[in]		light
						A reference to the omni light to copy.
		 */
		OmniLight(const OmniLight &light);

		/**
		 Constructs an omni light by moving the given omni light.

		 @param[in]		light
						A reference to the omni light to move.
		 */
		OmniLight(OmniLight &&light);

		/**
		 Destructs this omni light.
		 */
		virtual ~OmniLight();

		//---------------------------------------------------------------------
		// Assignment Operators
		//---------------------------------------------------------------------	

		/**
		 Copies the given omni light to this omni light.

		 @param[in]		light
						A reference to the omni light to copy.
		 @return		A reference to the copy of the given omni light (i.e. 
						this omni light).
		 */
		OmniLight &operator=(const OmniLight &light);

		/**
		 Moves the given omni light to this omni light.

		 @param[in]		light
						A reference to the omni light to move.
		 @return		A reference to the moved omni light (i.e. this omni 
						light).
		 */
		OmniLight &operator=(OmniLight &&light);

		//---------------------------------------------------------------------
		// Member Methods
		//---------------------------------------------------------------------

		/**
		 Clones this omni light.

		 @return		A pointer to the clone of this omni light.
		 */
		UniquePtr< OmniLight > Clone() const {
			return static_pointer_cast< OmniLight >(CloneImplementation());
		}
		
		//---------------------------------------------------------------------
		// Member Methods: Lighting
		//---------------------------------------------------------------------

		/**
		 Returns the power of this omni light.

		 @return		The power in watts of this omni light.
		 */
		F32 GetPower() const noexcept {
			return GetIntensity() * 0.25f * XM_1DIVPI;
		}

		/**
		 Sets the power of this omni light to the given radiance.
		 
		 @param[in]		power
						The power in watts.
		 */
		void SetPower(F32 power) noexcept {
			SetIntensity(power * 4.0f * XM_PI);
		}

		/**
		 Returns the power spectrum of this omni light.

		 @return		The power spectrum of this omni light.
		 */
		const RGB GetPowerSpectrum() const noexcept {
			const SRGB color   = GetBaseColor();
			const XMVECTOR P_v = GetPower()
				               * SRGBtoRGB(XMLoadFloat3(&color));
			RGB P;
			XMStoreFloat3(&P, P_v);
			return P;
		}

		/**
		 Returns the radiant intensity of this omni light.

		 @return		The radiant intensity in watts per steradians of this 
						omni light.
		 */
		F32 GetIntensity() const noexcept {
			return m_intensity;
		}

		/**
		 Sets the radiant intensity of this omni light to the given radial 
		 intensity.
		 
		 @param[in]		intensity
						The radiant intensity in watts per steradians.
		 */
		void SetIntensity(F32 intensity) noexcept {
			m_intensity = intensity;
		}

		/**
		 Returns the radiant intensity spectrum of this omni light.

		 @return		The radiant intensity spectrum of this omni light.
		 */
		const RGB GetIntensitySpectrum() const noexcept {
			const SRGB color   = GetBaseColor();
			const XMVECTOR I_v = GetIntensity()
				               * SRGBtoRGB(XMLoadFloat3(&color));
			RGB I;
			XMStoreFloat3(&I, I_v);
			return I;
		}

		//---------------------------------------------------------------------
		// Member Methods: Attenuation
		//---------------------------------------------------------------------

		/**
		 Returns the range of this omni light.

		 @return		The range of this omni light.
		 */
		F32 GetRange() const noexcept {
			return m_range;
		}

		/**
		 Sets the range of this omni light to the given value.

		 @pre			@a range must be greater than 0.
		 @param[in]		range
						The range.
		 */
		void SetRange(F32 range) noexcept {
			Assert(range > 0.0f);
			m_range = range;

			// Update the bounding volumes.
			UpdateBoundingVolumes();
		}

		//---------------------------------------------------------------------
		// Member Methods: Shadowing
		//---------------------------------------------------------------------

		/**
		 Checks whether shadows should be used for this omni light.

		 @return		@c true if shadows should be used for this omni light. 
						@c false otherwise.
		 */
		bool UseShadows() const noexcept {
			return m_shadows;
		}

		/**
		 Enables shadows for this omni light.
		 */
		void EnableShadows() noexcept {
			SetShadows(true);
		}

		/**
		 Dissables shadows for this omni light.
		 */
		void DissableShadows() noexcept {
			SetShadows(false);
		}

		/**
		 Toggles shadows for this omni light.
		 */
		void ToggleShadows() noexcept {
			SetShadows(!m_shadows);
		}
		
		/**
		 Sets shadows for this omni light to the given value.

		 @param[in]		shadows	
						@c true if shadows should be used for this omni light. 
						@c false otherwise.
		 */
		void SetShadows(bool shadows) noexcept {
			m_shadows = shadows;
		}

		/**
		 Returns the (horizontal and vertical) field-of-view of this omni 
		 light.

		 @return		The (horizontal and vertical) field-of-view of this 
						omni light.
		 */
		F32 GetFOV() const noexcept {
			return XM_PIDIV2;
		}

		/**
		 Returns the aspect ratio of this omni light.

		 @return		The aspect ratio of this omni light.
		 */
		F32 GetAspectRatio() const noexcept {
			return 1.0f;
		}

		/**
		 Returns the view-to-projection matrix of the (forward) light camera 
		 of this omni light.

		 @return		The view-to-projection matrix of the (forward) light 
						camera of this omni light.
		 */
		const XMMATRIX XM_CALLCONV GetViewToProjectionMatrix() const noexcept {
#ifdef DISSABLE_INVERTED_Z_BUFFER
			const F32 m22 = GetRange()
				/ (GetRange() - MAGE_DEFAULT_LIGHT_CAMERA_NEAR_Z);
			const F32 m32 = -MAGE_DEFAULT_LIGHT_CAMERA_NEAR_Z * m22;
#else  // DISSABLE_INVERTED_Z_BUFFER
			const F32 m22 = MAGE_DEFAULT_LIGHT_CAMERA_NEAR_Z 
				/ (MAGE_DEFAULT_LIGHT_CAMERA_NEAR_Z - GetRange());
			const F32 m32 = -GetRange() * m22;
#endif // DISSABLE_INVERTED_Z_BUFFER
			
			return XMMATRIX{
				1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f,
				0.0f, 0.0f,  m22, 1.0f,
				0.0f, 0.0f,  m32, 0.0f
			};
		}

		/**
		 Returns the (forward) light camera of this omni light.

		 @return		The (forward) light camera of this omni light.
		 */
		const PerspectiveCamera GetLightCamera() const noexcept {
			return PerspectiveCamera(
				GetAspectRatio(),
				GetFOV(),
				MAGE_DEFAULT_LIGHT_CAMERA_NEAR_Z,
				GetRange());
		}

	private:

		//---------------------------------------------------------------------
		// Member Methods
		//---------------------------------------------------------------------

		/**
		 Clones this omni light.

		 @return		A pointer to the clone of this omni light.
		 */
		virtual UniquePtr< Light > CloneImplementation() const override;

		/**
		 Updates the bounding volumes of this omni light.
		 */
		void UpdateBoundingVolumes() noexcept;

		//---------------------------------------------------------------------
		// Member Variables
		//---------------------------------------------------------------------

		/**
		 The radiant intensity in watts per steradians of this omni light.
		 */
		F32 m_intensity;

		/**
		 The range of this omni light.
		 */
		F32 m_range;

		/**
		 A flag indicating whether shadows should be calculated or not for 
		 this omni light.
		 */
		bool m_shadows;
	};
}