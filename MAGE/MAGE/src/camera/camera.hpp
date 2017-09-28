#pragma once

//-----------------------------------------------------------------------------
// Engine Includes
//-----------------------------------------------------------------------------
#pragma region

#include "math\math.hpp"

#pragma endregion

//-----------------------------------------------------------------------------
// Engine Defines
//-----------------------------------------------------------------------------
#pragma region

#define MAGE_DEFAULT_CAMERA_NEAR_Z        0.01f
#define MAGE_DEFAULT_CAMERA_FAR_Z        100.0f
#define MAGE_DEFAULT_LIGHT_CAMERA_NEAR_Z   0.1f

#pragma endregion

//-----------------------------------------------------------------------------
// Engine Declarations and Definitions
//-----------------------------------------------------------------------------
namespace mage {

	/**
	 A class of cameras.
	 */
	class Camera {

	public:

		//---------------------------------------------------------------------
		// Destructors
		//---------------------------------------------------------------------

		/**
		 Destructs this camera.
		 */
		virtual ~Camera();

		//---------------------------------------------------------------------
		// Assignment Operators
		//---------------------------------------------------------------------

		/**
		 Copies the given camera to this camera.

		 @param[in]		camera
						A reference to the camera to copy.
		 @return		A reference to the copy of the given camera (i.e. this 
						camera).
		 */
		Camera &operator=(const Camera &camera);

		/**
		 Moves the given camera to this camera.

		 @param[in]		camera
						A reference to the camera to move.
		 @return		A reference to the moved camera (i.e. this camera).
		 */
		Camera &operator=(Camera &&camera);

		//---------------------------------------------------------------------
		// Member Methods
		//---------------------------------------------------------------------

		/**
		 Clones this camera.

		 @return		A pointer to the clone of this camera.
		 */
		UniquePtr< Camera > Clone() const {
			return static_pointer_cast< Camera >(CloneImplementation());
		}

		/**
		 Returns the position of the near z-plane of this camera in camera 
		 space.

		 @return		The position of the near z-plane of this camera.
		 */
		f32 GetNearZ() const noexcept {
			return m_near_z;
		}
		
		/**
		 Sets the position of the near z-plane of this camera to the given 
		 value.

		 @param[in]		near_z
						The position of the near z-plane in camera space.
		 */
		void SetNearZ(f32 near_z) noexcept {
			m_near_z = near_z;
		}
		
		/**
		 Returns the position of the far z-plane of this camera in camera 
		 space.

		 @return		The position of the far z-plane of this camera.
		 */
		f32 GetFarZ() const noexcept {
			return m_far_z;
		}
		
		/**
		 Sets the position of the far z-plane of this camera to the given 
		 value.

		 @param[in]		far_z
						The position of the far z-plane in camera space.
		 */
		void SetFarZ(f32 far_z) noexcept {
			m_far_z = far_z;
		}
		
		/**
		 Sets the position of the near and far z-plane of this camera to the 
		 given values.

		 @param[in]		near_z
						The position of the near z-plane in camera space.
		 @param[in]		far_z
						The position of the far z-plane in camera space.
		 */
		void SetNearAndFarZ(f32 near_z, f32 far_z) noexcept {
			SetNearZ(near_z);
			SetFarZ(far_z);
		}

		/**
		 Returns the view-to-projection matrix of this camera.

		 @return		The view-to-projection matrix of this camera.
		 */
		virtual const XMMATRIX GetViewToProjectionMatrix() const noexcept = 0;

	protected:

		//---------------------------------------------------------------------
		// Constructors
		//---------------------------------------------------------------------

		/**
		 Constructs a camera.

		 @param[in]		near_z
						The position of the near z-plane in camera space.
		 @param[in]		far_z
						The position of the far z-plane in camera space.
		 */
		explicit Camera(
			f32 near_z = MAGE_DEFAULT_CAMERA_NEAR_Z,
			f32 far_z  = MAGE_DEFAULT_CAMERA_FAR_Z);

		/**
		 Constructs a camera from the given camera.

		 @param[in]		camera
						A reference to the camera to copy.
		 */
		Camera(const Camera &camera);

		/**
		 Constructs a camera by moving the given camera.

		 @param[in]		camera
						A reference to the camera to move.
		 */
		Camera(Camera &&camera);

	private:

		//---------------------------------------------------------------------
		// Member Methods
		//---------------------------------------------------------------------

		/**
		 Clones this camera.

		 @return		A pointer to the clone of this camera.
		 */
		virtual UniquePtr< Camera > CloneImplementation() const = 0;

		//---------------------------------------------------------------------
		// Member Variables
		//---------------------------------------------------------------------

		/**
		 The position of the near z-plane of this camera in camera space. 
		 */
		f32 m_near_z;

		/**
		 The position of the far z-plane of this camera in camera space.
		 */
		f32 m_far_z;
	};
}