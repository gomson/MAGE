#pragma once

//-----------------------------------------------------------------------------
// Engine Includes
//-----------------------------------------------------------------------------
#pragma region

#include "math\math.hpp"
#include "logging\error.hpp"

#pragma endregion

//-----------------------------------------------------------------------------
// Engine Declarations and Definitions
//-----------------------------------------------------------------------------
namespace mage {

	/**
	 Clamps the given value between the given low and hight value.

	 @pre			@a low is not greater than @a high.
	 @tparam		ValueT
					The type of value.
	 @param[in]		value
					The value.
	 @param[in]		low
					The minimum value.
	 @param[in]		high
					The maximum value.
	 @return		The clamped value between the given
					minimum and maximum value.
	 */
	template< typename ValueT >
	inline ValueT Clamp(ValueT value, ValueT low = 0, ValueT high = 1) noexcept {
		Assert(low <= high);
		
		return (value < high) ? ((value > low) ? value : low) : high;
	}

	/**
	 Clamps the given angle (in degrees) to (-180, 180].

	 @param[in]		angle
					The angle (in degrees).
	 @return		The clamped angle (in degrees).
	 */
	inline float ClampAngleDegrees(float angle) noexcept {
		while (angle < -180.0f) {
			angle += 360.0f;
		}
		while (angle > 180.0f) {
			angle -= 360.0f;
		}
		return angle;
	}

	/**
	 Clamps the given angle (in radians) to [-pi, pi].

	 @param[in]		angle
					The angle (in radians).
	 @return		The clamped angle (in radians).
	 */
	inline float ClampAngleRadians(float angle) noexcept {
		while (angle < -XM_PI) {
			angle += XM_2PI;
		}
		while (angle > XM_PI) {
			angle -= XM_2PI;
		}
		return angle;
	}

	/**
	 Clamps the given angle (in degrees) between the given
	 minimum and maximum angle (in degrees).

	 @pre			@a min_angle lies in [-180, 180].
	 @pre			@a max_angle lies in [-180, 180].
	 @pre			@a min_angle is not greater than @a max_angle.
	 @param[in]		angle
					The angle (in degrees).
	 @param[in]		min_angle
					The minimum angle (in degrees).
	 @param[in]		max_angle
					The maximum angle (in degrees).
	 @return		The clamped angle between the given
					minimum and maximum angle (in degrees).
	 */
	inline float ClampAngleDegrees(float angle, float min_angle, float max_angle) noexcept {
		Assert(min_angle <= max_angle);
		Assert(-XM_PI <= max_angle && max_angle <= XM_PI);
		Assert(-XM_PI <= max_angle && max_angle <= XM_PI);

		return Clamp(ClampAngleDegrees(angle), min_angle, max_angle);
	}

	/**
	 Clamps the given angle (in radians) between the given
	 minimum and maximum angle (in radians).

	 @pre			@a min_angle lies in [-pi, pi].
	 @pre			@a max_angle lies in [-pi, pi].
	 @pre			@a min_angle is not greater than @a max_angle.
	 @param[in]		angle
					The angle (in radians).
	 @param[in]		min_angle
					The minimum angle (in radians).
	 @param[in]		max_angle
					The maximum angle (in radians).
	 @return		The clamped angle between the given
					minimum and maximum angle (in radians).
	 */
	inline float ClampAngleRadians(float angle, float min_angle, float max_angle) noexcept {
		Assert(min_angle <= max_angle);
		Assert(-XM_PI <= max_angle && max_angle <= XM_PI);
		Assert(-XM_PI <= max_angle && max_angle <= XM_PI);
		
		return Clamp(ClampAngleRadians(angle), min_angle, max_angle);
	}

	/**
	 Returns the projection values from the given projection matrix
	 to reconstruct the view position coordinates from the NDC position coordinates.

	 @return		The projection values from the given projection matrix
					to reconstruct the view position coordinates from the NDC 
					position coordinates.
	 */
	inline const XMVECTOR XM_CALLCONV GetProjectionValues(FXMMATRIX projection_matrix) noexcept {
		//        [ 1/X  0   0  0 ]
		// p_view [  0  1/Y  0  0 ] = [p_view.x 1/X, p_view.y 1/Y, p_view.z (-W) + Z, p_view.z] = p_proj
		//        [  0   0  -W  1 ]
		//        [  0   0   Z  0 ]
		//
		// p_proj / p_proj.w        = [p_view.x/p_view.z 1/X, p_view.y/p_view.z 1/Y, (-W) + Z/p_view.z, 1] = p_ndc
		//
		// Reconstruction of p_view from p_ndc and projection values
		// 1) p_ndc.z = (-W) + Z/p_view.z     <=> p_view.z = Z / (p_ndc.z + W)
		// 2) p_ndc.x = p_view.x/p_view.z 1/X <=> p_view.x = X * p_ndc.x * p_view.z
		// 3) p_ndc.y = p_view.y/p_view.z 1/Y <=> p_view.y = Y * p_ndc.y * p_view.z

		const float x = 1.0f / XMVectorGetX(projection_matrix.r[0]);
		const float y = 1.0f / XMVectorGetY(projection_matrix.r[1]);
		const float z =  XMVectorGetZ(projection_matrix.r[3]);
		const float w = -XMVectorGetZ(projection_matrix.r[2]);
		return XMVectorSet(x, y, z, w);
	}
}