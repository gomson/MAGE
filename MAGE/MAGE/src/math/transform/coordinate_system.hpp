#pragma once

//-----------------------------------------------------------------------------
// Engine Includes
//-----------------------------------------------------------------------------
#pragma region

#include "math\math.hpp"

#pragma endregion

//-----------------------------------------------------------------------------
// Engine Declarations and Definitions
//-----------------------------------------------------------------------------
namespace mage {

	/**
	 Calculates an orthonormal basis from a given unit vector with the method 
	 of Hughes and M�ller.

	 @pre			The given vector is normalized.
	 @param[in]		n
					The first basis vector of the orthonormal basis.
	 @param[out]	b1
					The second basis vector of the orthonormal basis.
	 @param[out]	b2
					The third basis vector of the orthonormal basis.
	 */
	inline void XM_CALLCONV OrthonormalBasis_HughesMoller(
		FXMVECTOR n, XMVECTOR &b1, XMVECTOR &b2) noexcept {
		
		const XMVECTOR u = (fabs(XMVectorGetX(n)) > fabs(XMVectorGetZ(n)))
			? XMVectorSet(-XMVectorGetY(n), XMVectorGetX(n), 0.0f, 0.0f)
			: XMVectorSet(0.0f, -XMVectorGetZ(n), XMVectorGetY(n), 0.0f);
		b2 = XMVector3Normalize(u);
		b1 = XMVector3Cross(b2, n);
	}

	/**
	 Calculates an orthonormal basis from a given unit vector with the method 
	 of Frisvad.

	 @pre			The given vector is normalized.
	 @param[in]		n
					The first basis vector of the orthonormal basis.
	 @param[out]	b1
					The second basis vector of the orthonormal basis.
	 @param[out]	b2
					The third basis vector of the orthonormal basis.
	 */
	inline void XM_CALLCONV OrthonormalBasis_Frisvad(
		FXMVECTOR n, XMVECTOR &b1, XMVECTOR &b2) noexcept {
		
		F32x3 nf;
		XMStoreFloat3(&nf, n);

		if (nf.m_z < -0.9999999f) {
			b1 = XMVectorSet( 0.0f, -1.0f, 0.0f, 0.0f);
			b2 = XMVectorSet(-1.0f,  0.0f, 0.0f, 0.0f);
			return;
		}

		const F32 a = 1.0f / (1.0f + nf.m_z);
		const F32 b = -nf.m_x * nf.m_y * a;
		b1 = XMVectorSet(1.0f - nf.m_x * nf.m_x * a, 
			             b, 
			             -nf.m_x, 0.0f);
		b2 = XMVectorSet(b, 
			             1.0f - nf.m_y * nf.m_y * a, 
			             -nf.m_y, 0.0f);
	}

	/**
	 Calculates an orthonormal basis from a given unit vector with the method 
	 of Duff, Burgess, Christensen, Hery, Kensler, Liani and Villemin.

	 @pre			The given vector is normalized.
	 @param[in]		n
					The first basis vector of the orthonormal basis.
	 @param[out]	b1
					The second basis vector of the orthonormal basis.
	 @param[out]	b2
					The third basis vector of the orthonormal basis.
	 */
	inline void XM_CALLCONV OrthonormalBasis_Duff(
		FXMVECTOR n, XMVECTOR &b1, XMVECTOR &b2) noexcept {
		
		F32x3 nf;
		XMStoreFloat3(&nf, n);

		const F32 sign = copysignf(1.0f, nf.m_z);
		const F32 a = -1.0f / (sign + nf.m_z);
		const F32 b = nf.m_x * nf.m_y * a;
		b1 = XMVectorSet(1.0f + sign * nf.m_x * nf.m_x * a, 
			             sign * b, 
			             -sign * nf.m_x, 0.0f);
		b2 = XMVectorSet(b, sign + nf.m_y * nf.m_y * a, 
			             -nf.m_y, 
			             0.0f);
	}

	/**
	 Calculates an orthonormal basis from a given unit vector.

	 @pre			The given vector is normalized.
	 @param[in]		n
					The first basis vector of the orthonormal basis.
	 @param[out]	b1
					The second basis vector of the orthonormal basis.
	 @param[out]	b2
					The third basis vector of the orthonormal basis.
	 */
	inline void XM_CALLCONV OrthonormalBasis(
		FXMVECTOR n, XMVECTOR &b1, XMVECTOR &b2) noexcept {
		
		OrthonormalBasis_Duff(n, b1, b2);
	}

	//-------------------------------------------------------------------------
	// CartesianAxesSystem
	//-------------------------------------------------------------------------

	/**
	 A struct of Cartesian axes systems.
	 */
	struct alignas(16) CartesianAxesSystem final {

	public:

		//---------------------------------------------------------------------
		// Constructors and Destructors
		//---------------------------------------------------------------------

		/**
		 Constructs a Cartesian axes system.
		 */
		CartesianAxesSystem()
			: m_x(XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f)), 
			  m_y(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)), 
			  m_z(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f)) {}
		
		/**
		 Constructs a Cartesian axes system from the given axes.

		 @pre			The given axis is normalized.
		 @param[in]		x
						The x-axis.
		 */
		explicit CartesianAxesSystem(FXMVECTOR x) 
			: m_x(x) {
			
			OrthonormalBasis(m_x, m_y, m_z);
		}

		/**
		 Constructs a Cartesian axes system from the given axes.

		 @pre			The given axes are orthonormal.
		 @param[in]		x
						The x-axis.
		 @param[in]		y
						The y-axis.
		 */
		explicit CartesianAxesSystem(FXMVECTOR x, FXMVECTOR y) 
			: m_x(x), m_y(y), m_z(XMVector3Cross(x, y)) {}

		/**
		 Constructs a Cartesian axes system from the given axes.

		 @pre			The given axes are orthonormal.
		 @param[in]		x
						The x-axis.
		 @param[in]		y
						The y-axis.
		 @param[in]		z
						The z-axis.
		 */
		explicit CartesianAxesSystem(FXMVECTOR x, FXMVECTOR y, FXMVECTOR z) 
			: m_x(x), m_y(y), m_z(z) {}

		/**
		 Constructs a Cartesian axes system from the given Cartesian axes 
		 system.

		 @param[in]		axes
						A reference to the Cartesian axes system to copy.
		 */
		CartesianAxesSystem(const CartesianAxesSystem &axes) = default;

		/**
		 Constructs a Cartesian axes system by moving the given Cartesian axes 
		 system.

		 @param[in]		axes
						A reference to the Cartesian axes system to move.
		 */
		CartesianAxesSystem(CartesianAxesSystem &&axes) = default;

		/**
		 Destructs this Cartesian axes system.
		 */
		~CartesianAxesSystem() = default;

		//---------------------------------------------------------------------
		// Assignment Operators
		//---------------------------------------------------------------------

		/**
		 Copies the given Cartesian axes system to this Cartesian axes system.

		 @param[in]		axes
						A reference to the Cartesian axes system to copy.
		 @return		A reference to the copy of the given Cartesian axes 
						system (i.e. this Cartesian axes system).
		 */
		CartesianAxesSystem &operator=(
			const CartesianAxesSystem &axes) = default;

		/**
		 Moves the given Cartesian axes system to this Cartesian axes system.

		 @param[in]		axes
						A reference to the Cartesian axes system to move.
		 @return		A reference to the moved Cartesian axes system (i.e. 
						this Cartesian axes system).
		 */
		CartesianAxesSystem &operator=(
			CartesianAxesSystem &&axes) = default;

		//---------------------------------------------------------------------
		// Member Methods
		//---------------------------------------------------------------------

		/**
		 Returns the x-axis of this Cartesian axes system.

		 @return		The x-axis of this Cartesian axes system.
		 */
		const XMVECTOR XM_CALLCONV GetAxisX() const noexcept {
			return m_x;
		}

		/**
		 Returns the y-axis of this Cartesian axes system.

		 @return		The y-axis of this Cartesian axes system.
		 */
		const XMVECTOR XM_CALLCONV GetAxisY() const noexcept {
			return m_y;
		}

		/**
		 Returns the z-axis of this Cartesian axes system.

		 @return		The z-axis of this Cartesian axes system.
		 */
		const XMVECTOR XM_CALLCONV GetAxisZ() const noexcept {
			return m_z;
		}

	private:

		//---------------------------------------------------------------------
		// Member Variables
		//---------------------------------------------------------------------

		/**
		 The x-axis of this Cartesian axes system.
		 */
		XMVECTOR m_x;

		/**
		 The y-axis of this Cartesian axes system.
		 */
		XMVECTOR m_y;

		/**
		 The z-axis of this Cartesian axes system.
		 */
		XMVECTOR m_z;
	};

	//-------------------------------------------------------------------------
	// CartesianCoordinateSystem
	//-------------------------------------------------------------------------

	/**
	 A struct of Cartesian coordinate systems.
	 */
	struct alignas(16) CartesianCoordinateSystem final {

	public:

		//---------------------------------------------------------------------
		// Constructors and Destructors
		//---------------------------------------------------------------------

		/**
		 Constructs a Cartesian coordinate system from the given Cartesian axes 
		 system.

		 @param[in]		axes
						A reference to the Cartesian axes system.
		 */
		explicit CartesianCoordinateSystem(const CartesianAxesSystem &axes)
			: m_o(XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f)), m_axes(axes) {}

		/**
		 Constructs a Cartesian coordinate system from the given origin and 
		 Cartesian axes system.

		 @param[in]		o
						The origin.
		 @param[in]		axes
						A reference to the Cartesian axes system.
		 */
		explicit CartesianCoordinateSystem(
			FXMVECTOR o, const CartesianAxesSystem &axes)
			: m_o(o), m_axes(axes) {}

		/**
		 Constructs a Cartesian coordinate system from the given Cartesian 
		 coordinate system.

		 @param[in]		coordinate_system
						A reference to the Cartesian coordinate system.
		 */
		CartesianCoordinateSystem(
			const CartesianCoordinateSystem &coordinate_system) = default;

		/**
		 Constructs a Cartesian coordinate system by moving the given Cartesian 
		 coordinate system.

		 @param[in]		coordinate_system
						A reference to the Cartesian coordinate system.
		 */
		CartesianCoordinateSystem(
			CartesianCoordinateSystem &&coordinate_system) = default;

		/**
		 Destructs this Cartesian coordinate system.
		 */
		~CartesianCoordinateSystem() = default;

		//---------------------------------------------------------------------
		// Assignment Operators
		//---------------------------------------------------------------------

		/**
		 Copies the given Cartesian coordinate system to this Cartesian 
		 coordinate system.

		 @param[in]		coordinate_system
						A reference to the Cartesian coordinate system to copy.
		 @return		A reference to the copy of the given Cartesian 
						coordinate system (i.e. this Cartesian coordinate 
						system).
		 */
		CartesianCoordinateSystem &operator=(
			const CartesianCoordinateSystem &coordinate_system) = default;

		/**
		 Moves the given Cartesian coordinate system to this Cartesian 
		 coordinate system.

		 @param[in]		coordinate_system
						A reference to the Cartesian coordinate system to move.
		 @return		A reference to the moved Cartesian coordinate system 
						(i.e. this Cartesian coordinate system).
		 */
		CartesianCoordinateSystem &operator=(
			CartesianCoordinateSystem &&coordinate_system) = default;

		//---------------------------------------------------------------------
		// Member Methods
		//---------------------------------------------------------------------

		/**
		 Returns the origin of this Cartesian coordinate system.

		 @return		The origin of this Cartesian coordinate system.
		 */
		const XMVECTOR XM_CALLCONV GetOrigin() const noexcept {
			return m_o;
		}

		/**
		 Returns the x-axis of this Cartesian coordinate system.

		 @return		The x-axis of this Cartesian coordinate system.
		 */
		const XMVECTOR XM_CALLCONV GetAxisX() const noexcept {
			return m_axes.GetAxisX();
		}

		/**
		 Returns the y-axis of this Cartesian coordinate system.

		 @return		The y-axis of this Cartesian coordinate system.
		 */
		const XMVECTOR XM_CALLCONV GetAxisY() const noexcept {
			return m_axes.GetAxisY();
		}

		/**
		 Returns the z-axis of this Cartesian coordinate system.

		 @return		The z-axis of this Cartesian coordinate system.
		 */
		const XMVECTOR XM_CALLCONV GetAxisZ() const noexcept {
			return m_axes.GetAxisZ();
		}
		
		/**
		 Returns the axes of this Cartesian coordinate system.

		 @return		The Cartesian axes system of this Cartesian coordinate 
						system.
		 */
		const CartesianAxesSystem GetAxes() const noexcept {
			return m_axes;
		}

	private:

		//---------------------------------------------------------------------
		// Member Variables
		//---------------------------------------------------------------------

		/**
		 The origin of this Cartesian coordinate system.
		 */
		XMVECTOR m_o;

		/**
		 The Cartesian axes system of this Cartesian coordinate system.
		 */
		CartesianAxesSystem m_axes;
	};
}
