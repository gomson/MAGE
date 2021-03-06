//-----------------------------------------------------------------------------
// Engine Includes
//-----------------------------------------------------------------------------
#pragma region

#include "math\geometry\view_frustum.hpp"

#pragma endregion

//-----------------------------------------------------------------------------
// Engine Definitions
//-----------------------------------------------------------------------------
namespace mage {

	ViewFrustum::ViewFrustum(CXMMATRIX transform) {
		const XMMATRIX C = XMMatrixTranspose(transform);

		// Extract the view frustum planes from the given transform.
		// All view frustum planes are inward facing: 0 <= n . p + d

		// p' = (x',y',z',w') = (x,y,z,1) T = p T

		//   -w' <= x'
		// <=> 0 <= w' + x'
		// <=> 0 <= p . c3 + p . c0
		// <=> 0 <= p . (c3 + c0)
		m_left_plane = C.r[3] + C.r[0];
		
		//    x' <= w'
		// <=> 0 <= w' - x'
		// <=> 0 <= p . c3 - p . c0
		// <=> 0 <= p . (c3 - c0)
		m_right_plane = C.r[3] - C.r[0];
		
		//   -w' <= y'
		// <=> 0 <= w' + y'
		// <=> 0 <= p . c3 + p . c1
		// <=> 0 <= p . (c3 + c1)
		m_bottom_plane = C.r[3] + C.r[1];
		
		//    y' <= w'
		// <=> 0 <= w' - y'
		// <=> 0 <= p . c3 - p . c1
		// <=> 0 <= p . (c3 - c1)
		m_top_plane = C.r[3] - C.r[1];
		
		//     0 <= z'
		// <=> 0 <= p . c2
		m_near_plane = C.r[2];

		//    z' <= w'
		// <=> 0 <= w' - z'
		// <=> 0 <= p . c3 - p . c2
		// <=> 0 <= p . (c3 - c2)
		m_far_plane = C.r[3] - C.r[2];

		// Normalize the view frustum planes.
		for (size_t i = 0; i < 6; ++i) {
			m_planes[i] = XMPlaneNormalize(m_planes[i]);
		}
	}

	/**
	 Calculates the minimum point of a given AABB along a given normal.

	 @param[in]		n
					The normal.
	 @param[in]		aabb
					A reference to the AABB.
	 @param[out]	pmin
					A reference to the minimum point of @a aabb along @a n.
	 @param[out]	pmax
					A reference to the maximum point of @a aabb along @a n.
	 */
	inline void XM_CALLCONV MinAndMaxPointAlongNormal(FXMVECTOR n, 
		const AABB &aabb, XMVECTOR &pmin, XMVECTOR &pmax) noexcept {
		
		const XMVECTOR control   = XMVectorGreaterOrEqual(n, XMVectorZero());
		const XMVECTOR aabb_pmin = XMLoadFloat3(&aabb.m_p_min);
		const XMVECTOR aabb_pmax = XMLoadFloat3(&aabb.m_p_max);
		
		pmin = XMVectorSetW(XMVectorSelect(aabb_pmax, aabb_pmin, control), 1.0f);
		pmax = XMVectorSetW(XMVectorSelect(aabb_pmin, aabb_pmax, control), 1.0f);
	}

	/**
	 Calculates the minimum point of a given AABB along a given normal.

	 @param[in]		n
					The normal.
	 @param[in]		aabb
					A reference to the AABB.
	 @return		The minimum point of @a aabb along @a n.
	 */
	inline XMVECTOR XM_CALLCONV MinPointAlongNormal(FXMVECTOR n, 
		const AABB &aabb) noexcept {
		
		const XMVECTOR control   = XMVectorGreaterOrEqual(n, XMVectorZero());
		const XMVECTOR aabb_pmin = XMLoadFloat3(&aabb.m_p_min);
		const XMVECTOR aabb_pmax = XMLoadFloat3(&aabb.m_p_max);

		return XMVectorSetW(XMVectorSelect(aabb_pmax, aabb_pmin, control), 1.0f);
	}

	/**
	 Calculates the maximum point of a given AABB along a given normal.

	 @param[in]		n
					The normal.
	 @param[in]		aabb
					A reference to the AABB.
	 @return		The minimum point of @a aabb along @a n.
	 */
	inline XMVECTOR XM_CALLCONV MaxPointAlongNormal(FXMVECTOR n, 
		const AABB &aabb) noexcept {
		
		const XMVECTOR control   = XMVectorGreaterOrEqual(n, XMVectorZero());
		const XMVECTOR aabb_pmin = XMLoadFloat3(&aabb.m_p_min);
		const XMVECTOR aabb_pmax = XMLoadFloat3(&aabb.m_p_max);

		return XMVectorSetW(XMVectorSelect(aabb_pmin, aabb_pmax, control), 1.0f);
	}

	//-------------------------------------------------------------------------
	// ViewFrustum: Enclosing = Full Coverage
	//-------------------------------------------------------------------------

	bool ViewFrustum::Encloses(const Point3 &point) const noexcept {
		return Encloses(XMLoadFloat3(&point));
	}

	bool ViewFrustum::EnclosesStrict(const Point3 &point) const noexcept {
		return EnclosesStrict(XMLoadFloat3(&point));
	}

	bool XM_CALLCONV ViewFrustum::Encloses(FXMVECTOR point) const noexcept {
		for (size_t i = 0; i < 6; ++i) {
			const XMVECTOR result = XMPlaneDotCoord(m_planes[i], point);
			if (XMVectorGetX(result) < 0.0f) {
				return false;
			}
		}

		return true;
	}

	bool XM_CALLCONV ViewFrustum::EnclosesStrict(FXMVECTOR point) const noexcept {
		for (size_t i = 0; i < 6; ++i) {
			const XMVECTOR result = XMPlaneDotCoord(m_planes[i], point);
			if (XMVectorGetX(result) <= 0.0f) {
				return false;
			}
		}

		return true;
	}

	bool ViewFrustum::Encloses(const AABB &aabb) const noexcept {
		for (size_t i = 0; i < 6; ++i) {
			const XMVECTOR p = MinPointAlongNormal(m_planes[i], aabb);
			const XMVECTOR result = XMPlaneDotCoord(m_planes[i], p);
			if (XMVectorGetX(result) < 0.0f) {
				return false;
			}
		}

		return true;
	}

	bool ViewFrustum::EnclosesStrict(const AABB &aabb) const noexcept {
		for (size_t i = 0; i < 6; ++i) {
			const XMVECTOR p = MinPointAlongNormal(m_planes[i], aabb);
			const XMVECTOR result = XMPlaneDotCoord(m_planes[i], p);
			if (XMVectorGetX(result) <= 0.0f) {
				return false;
			}
		}

		return true;
	}

	bool ViewFrustum::Encloses(const BS &bs) const noexcept {
		for (size_t i = 0; i < 6; ++i) {
			const XMVECTOR p = XMLoadFloat3(&bs.m_p);
			const XMVECTOR result = XMPlaneDotCoord(m_planes[i], p);
			if (XMVectorGetX(result) < bs.m_r) {
				return false;
			}
		}

		return true;
	}

	bool ViewFrustum::EnclosesStrict(const BS &bs) const noexcept {
		for (size_t i = 0; i < 6; ++i) {
			const XMVECTOR p = XMLoadFloat3(&bs.m_p);
			const XMVECTOR result = XMPlaneDotCoord(m_planes[i], p);
			if (XMVectorGetX(result) <= bs.m_r) {
				return false;
			}
		}

		return true;
	}

	//-------------------------------------------------------------------------
	// ViewFrustum: Overlapping = Partial | Full Coverage
	//-------------------------------------------------------------------------

	bool ViewFrustum::Overlaps(const AABB &aabb) const noexcept {
		// Test for no coverage.
		for (size_t i = 0; i < 6; ++i) {
			const XMVECTOR p = MaxPointAlongNormal(m_planes[i], aabb);
			const XMVECTOR result = XMPlaneDotCoord(m_planes[i], p);
			if (XMVectorGetX(result) < 0.0f) {
				return false;
			}
		}

		return true;
	}

	bool ViewFrustum::OverlapsStrict(const AABB &aabb) const noexcept {
		// Test for no coverage.
		for (size_t i = 0; i < 6; ++i) {
			const XMVECTOR p = MaxPointAlongNormal(m_planes[i], aabb);
			const XMVECTOR result = XMPlaneDotCoord(m_planes[i], p);
			if (XMVectorGetX(result) <= 0.0f) {
				return false;
			}
		}

		return true;
	}

	bool ViewFrustum::Overlaps(const BS &bs) const noexcept {
		// Test for no coverage.
		for (size_t i = 0; i < 6; ++i) {
			const XMVECTOR p = XMLoadFloat3(&bs.m_p);
			const XMVECTOR result = XMPlaneDotCoord(m_planes[i], p);
			if (XMVectorGetX(result) < -bs.m_r) {
				return false;
			}
		}

		return true;
	}

	bool ViewFrustum::OverlapsStrict(const BS &bs) const noexcept {
		// Test for no coverage.
		for (size_t i = 0; i < 6; ++i) {
			const XMVECTOR p = XMLoadFloat3(&bs.m_p);
			const XMVECTOR result = XMPlaneDotCoord(m_planes[i], p);
			if (XMVectorGetX(result) <= -bs.m_r) {
				return false;
			}
		}

		return true;
	}

	//-------------------------------------------------------------------------
	// ViewFrustum: Intersecting = Partial Coverage
	//-------------------------------------------------------------------------

	bool ViewFrustum::Intersects(const Point3 &point, 
		F32 epsilon) const noexcept {
		
		return Intersects(XMLoadFloat3(&point), epsilon);
	}

	bool XM_CALLCONV ViewFrustum::Intersects(FXMVECTOR point,
		F32 epsilon) const noexcept {
		
		bool intersection = false;
		for (size_t i = 0; i < 6; ++i) {
			const XMVECTOR result = XMPlaneDotCoord(m_planes[i], point);
			const F32 distance = XMVectorGetX(result);

			// Test for no coverage.
			if (distance < -epsilon) {
				return false;
			}

			// Test for partial coverage.
			intersection |= (fabs(distance) <= epsilon);
		}

		return intersection;
	}

	bool ViewFrustum::Intersects(const AABB &aabb) const noexcept {
		bool intersection = false;
		for (size_t i = 0; i < 6; ++i) {
			XMVECTOR pmin, pmax;
			MinAndMaxPointAlongNormal(m_planes[i], aabb, pmin, pmax);

			// Test for no coverage.
			const XMVECTOR result_max = XMPlaneDotCoord(m_planes[i], pmax);
			const F32 distance_max = XMVectorGetX(result_max);
			if (distance_max < 0.0f) {
				return false;
			}

			// Test for partial coverage.
			const XMVECTOR result_min = XMPlaneDotCoord(m_planes[i], pmin);
			const F32 distance_min = XMVectorGetX(result_min);
			intersection |= (distance_min <= 0.0f);
		}

		return intersection;
	}

	bool ViewFrustum::Intersects(const BS &bs) const noexcept {
		bool intersection = false;
		for (size_t i = 0; i < 6; ++i) {
			const XMVECTOR p = XMLoadFloat3(&bs.m_p);
			const XMVECTOR result = XMPlaneDotCoord(m_planes[i], p);
			const F32 distance = XMVectorGetX(result);

			// Test for no coverage.
			if (distance < -bs.m_r) {
				return false;
			}

			// Test for partial coverage.
			intersection |= (fabs(distance) <= bs.m_r);
		}

		return intersection;
	}

	//---------------------------------------------------------------------
	// Member Methods: Classification
	//---------------------------------------------------------------------

	Coverage ViewFrustum::Classify(const Point3 &point, 
		F32 epsilon) const noexcept {
		
		return Classify(XMLoadFloat3(&point), epsilon);
	}

	Coverage XM_CALLCONV ViewFrustum::Classify(FXMVECTOR point,
		F32 epsilon) const noexcept {
		
		bool intersection = false;
		for (size_t i = 0; i < 6; ++i) {
			const XMVECTOR result = XMPlaneDotCoord(m_planes[i], point);
			const F32 distance = XMVectorGetX(result);

			// Test for no coverage.
			if (distance < -epsilon) {
				return Coverage::NoCoverage;
			}

			// Test for partial coverage.
			intersection |= (fabs(distance) <= epsilon);
		}

		return intersection ? Coverage::PartialCoverage : Coverage::FullCoverage;
	}

	Coverage ViewFrustum::Classify(const AABB &aabb) const noexcept {
		bool intersection = false;
		for (size_t i = 0; i < 6; ++i) {
			XMVECTOR pmin, pmax;
			MinAndMaxPointAlongNormal(m_planes[i], aabb, pmin, pmax);

			// Test for no coverage.
			const XMVECTOR result_max = XMPlaneDotCoord(m_planes[i], pmax);
			const F32 distance_max = XMVectorGetX(result_max);
			if (distance_max < 0.0f) {
				return Coverage::NoCoverage;
			}

			// Test for partial coverage.
			const XMVECTOR result_min = XMPlaneDotCoord(m_planes[i], pmin);
			const F32 distance_min = XMVectorGetX(result_min);
			intersection |= (distance_min <= 0.0f);
		}

		return intersection ? Coverage::PartialCoverage : Coverage::FullCoverage;
	}

	Coverage ViewFrustum::Classify(const BS &bs) const noexcept {
		bool intersection = false;
		for (size_t i = 0; i < 6; ++i) {
			const XMVECTOR p = XMLoadFloat3(&bs.m_p);
			const XMVECTOR result = XMPlaneDotCoord(m_planes[i], p);
			const F32 distance = XMVectorGetX(result);

			// Test for no coverage.
			if (distance < -bs.m_r) {
				return Coverage::NoCoverage;
			}

			// Test for partial coverage.
			intersection |= (fabs(distance) <= bs.m_r);
		}

		return intersection ? Coverage::PartialCoverage : Coverage::FullCoverage;
	}
}