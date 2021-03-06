#pragma once

//-----------------------------------------------------------------------------
// Engine Includes
//-----------------------------------------------------------------------------
#pragma region

#include "math\geometry\bounding_volume.hpp"

#pragma endregion

//-----------------------------------------------------------------------------
// Engine Declarations and Definitions
//-----------------------------------------------------------------------------
namespace mage {

	/**
	 A struct of view frustums.
	 */
	struct alignas(16) ViewFrustum final {

	public:

		/**
		 Checks if the given AABB is culled by the view frustum constructed 
		 from the given object-to-projection transformation matrix.

		 @param[in]		object_to_projection
						The object-to-projection transformation matrix.
		 @param[in]		aabb
						A reference to the AABB.
		 @return		@c true if the given AABB is culled by the view frustum 
						constructed from the given object-to-projection 
						transformation matrix. @c false otherwise.
		 */
		static bool XM_CALLCONV Cull(FXMMATRIX object_to_projection, 
			const AABB &aabb) noexcept {
			const ViewFrustum view_frustum(object_to_projection);
			return !view_frustum.Overlaps(aabb);
		}

		/**
		 Checks if the given BS is culled by the view frustum constructed from 
		 the given object-to-projection transformation matrix.

		 @param[in]		object_to_projection
						The object-to-projection transformation matrix.
		 @param[in]		bs
						A reference to the BS.
		 @return		@c true if the given BS is culled by the view frustum 
						constructed from the given object-to-projection 
						transformation matrix. @c false otherwise.
		 */
		static bool XM_CALLCONV Cull(FXMMATRIX object_to_projection, 
			const BS &bs) noexcept {
			const ViewFrustum view_frustum(object_to_projection);
			return !view_frustum.Overlaps(bs);
		}

		//---------------------------------------------------------------------
		// Constructors and Destructors
		//---------------------------------------------------------------------
		
		/**
		 Constructs a view frustum from the given transform.
		 
		 If the given transform represents the view-to-projection matrix, the 
		 planes of the view frustum are represented by view space coordinates.

		 If the given transform represents the world-to-view-to-projection 
		 matrix, the planes of the view frustum are represented by world space 
		 coordinates.

		 If the given transform represents the object-to-world-to-view-projection 
		 matrix, the planes of the view frustum are represented by object space 
		 coordinates.
		 */
		explicit ViewFrustum(CXMMATRIX transform);

		/**
		 Constructs a view frustum from the given view frustum.

		 @param[in]		view_frustum
						A reference to the view frustum to copy.
		 */
		ViewFrustum(const ViewFrustum &view_frustum) = default;

		/**
		 Constructs a view frustum by moving the given view frustum.

		 @param[in]		view_frustum
						A reference to the view frustum to move.
		 */
		ViewFrustum(ViewFrustum &&view_frustum) = default;

		/**
		 Destructs this view frustum.
		 */
		~ViewFrustum() = default;

		//---------------------------------------------------------------------
		// Assignment Operators
		//---------------------------------------------------------------------

		/**
		 Copies the given view frustum to this view frustum.

		 @param[in]		view_frustum
						A reference to the view frustum to copy.
		 @return		A reference to the copy of the given view frustum (i.e. 
						this view frustum).
		 */
		ViewFrustum &operator=(const ViewFrustum &view_frustum) = default;

		/**
		 Moves the given view frustum to this view frustum.

		 @param[in]		view_frustum
						A reference to the view frustum to move.
		 @return		A reference to the moved view frustum (i.e. this view 
						frustum).
		 */
		ViewFrustum &operator=(ViewFrustum &&view_frustum) = default;
		
		//---------------------------------------------------------------------
		// Member Methods: Enclosing = Full Coverage
		//---------------------------------------------------------------------
		
		/**
		 Checks whether this view frustum completely encloses the given point.

		 @param[in]		point
						A reference to the point.
		 @return		@c true if this view frustum completely encloses 
						@a point. @c false otherwise.
		 @note			This is a full coverage test of a point with regard to 
						a view frustum.
		 */
		bool Encloses(const Point3 &point) const noexcept;

		/**
		 Checks whether this view frustum completely, strictly encloses the 
		 given point.

		 @param[in]		point
						A reference to the point.
		 @return		@c true if this view frustum completely, strictly encloses 
						@a point. @c false otherwise.
		 @note			This is a full coverage test of a point with regard to 
						a view frustum.
		 */
		bool EnclosesStrict(const Point3 &point) const noexcept;

		/**
		 Checks whether this view frustum completely encloses the given point.

		 @param[in]		point
						The point.
		 @return		@c true if this view frustum completely encloses 
						@a point. @c false otherwise.
		 @note			This is a full coverage test of a point with regard to 
						a view frustum.
		 */
		bool XM_CALLCONV Encloses(FXMVECTOR point) const noexcept;

		/**
		 Checks whether this view frustum completely, strictly encloses the 
		 given point.

		 @param[in]		point
						The point.
		 @return		@c true if this view frustum completely, strictly 
						encloses @a point. @c false otherwise.
		 @note			This is a full coverage test of a point with regard to 
						a view frustum.
		 */
		bool XM_CALLCONV EnclosesStrict(FXMVECTOR point) const noexcept;

		/**
		 Checks whether this view frustum completely encloses the given AABB.

		 @param[in]		aabb
						A reference to the AABB.
		 @return		@c true if this view frustum completely encloses 
						@a aabb. @c false otherwise.
		 @note			This is a full coverage test of an AABB with regard to 
						a view frustum.
		 */
		bool Encloses(const AABB &aabb) const noexcept;

		/**
		 Checks whether this view frustum completely, strictly encloses the 
		 given AABB.

		 @param[in]		aabb
						A reference to the AABB.
		 @return		@c true if this view frustum completely, strictly 
						encloses @a aabb. @c false otherwise.
		 @note			This is a full coverage test of an AABB with regard to 
						a view frustum.
		 */
		bool EnclosesStrict(const AABB &aabb) const noexcept;

		/**
		 Checks whether this view frustum completely encloses the given BS.

		 @param[in]		bs
						A reference to the BS.
		 @return		@c true if this view frustum completely encloses @a bs.
						@c false otherwise.
		 @note			This is a full coverage test of a BS with regard to a 
						view frustum.
		 */
		bool Encloses(const BS &bs) const noexcept;

		/**
		 Checks whether this view frustum completely, strictly encloses the given BS.

		 @param[in]		bs
						A reference to the BS.
		 @return		@c true if this view frustum completely, strictly 
						encloses @a bs. @c false otherwise.
		 @note			This is a full coverage test of a BS with regard to a 
						view frustum.
		 */
		bool EnclosesStrict(const BS &bs) const noexcept;
		
		//---------------------------------------------------------------------
		// Member Methods: Overlapping = Partial | Full Coverage
		//---------------------------------------------------------------------
		
		/**
		 Checks whether this view frustum overlaps the given AABB.

		 @param[in]		aabb
						A reference to the AABB.
		 @return		@c true if this view frustum overlaps @a aabb.
						@c false otherwise.
		 @note			This is a (partial or full) coverage test of an AABB 
						with regard to a view frustum.
		 */
		bool Overlaps(const AABB &aabb) const noexcept;

		/**
		 Checks whether this view frustum strictly overlaps the given AABB.

		 @param[in]		aabb
						A reference to the AABB.
		 @return		@c true if this view frustum strictly overlaps @a aabb.
						@c false otherwise.
		 @note			This is a (partial or full) coverage test of an AABB 
						with regard to a view frustum.
		 */
		bool OverlapsStrict(const AABB &aabb) const noexcept;

		/**
		 Checks whether this view frustum overlaps the given BS.

		 @param[in]		bs
						A reference to the BS.
		 @return		@c true if this view frustum overlaps @a bs.
						@c false otherwise.
		 @note			This is a (partial or full) coverage test of a BS with 
						regard to a view frustum.
		 */
		bool Overlaps(const BS &bs) const noexcept;

		/**
		 Checks whether this view frustum strictly overlaps the given BS.

		 @param[in]		bs
						A reference to the BS.
		 @return		@c true if this view frustum strictly overlaps @a bs.
						@c false otherwise.
		 @note			This is a (partial or full) coverage test of a BS with 
						regard to a view frustum.
		 */
		bool OverlapsStrict(const BS &bs) const noexcept;

		//---------------------------------------------------------------------
		// Member Methods: Intersecting = Partial Coverage
		//---------------------------------------------------------------------

		/**
		 Checks whether this view frustum intersects the given point.

		 @param[in]		point
						A reference to the point.
		 @param[in]		epsilon
						The epsilon value for F32 comparisons.
		 @return		@c true if this view frustum intersects @a point.
						@c false otherwise.
		 @note			This is a partial coverage test of a point with regard 
						to a view frustum.
		 */
		bool Intersects(const Point3 &point, 
			F32 epsilon = 0.0f) const noexcept;

		/**
		 Checks whether this view frustum intersects the given point.

		 @param[in]		point
						The point.
		 @param[in]		epsilon
						The epsilon value for F32 comparisons.
		 @return		@c true if this view frustum intersects @a point.
						@c false otherwise.
		 @note			This is a partial coverage test of a point with regard 
						to a view frustum.
		 */
		bool XM_CALLCONV Intersects(FXMVECTOR point,
			F32 epsilon = 0.0f) const noexcept;

		/**
		 Checks whether this view frustum intersects the given AABB.

		 @param[in]		aabb
						A reference to the AABB.
		 @return		@c true if this view frustum intersects @a aabb.
						@c false otherwise.
		 @note			This is a partial coverage test of an AABB with regard 
						to a view frustum.
		 */
		bool Intersects(const AABB &aabb) const noexcept;

		/**
		 Checks whether this view frustum intersects the given BS.

		 @param[in]		bs
						A reference to the BS.
		 @return		@c true if this view frustum intersects @a bs.
						@c false otherwise.
		 @note			This is a partial coverage test of a BS with regard to 
						a view frustum.
		 */
		bool Intersects(const BS &bs) const noexcept;

		//---------------------------------------------------------------------
		// Member Methods: Classification
		//---------------------------------------------------------------------

		/**
		 Classifies the coverage of the given point with regard to this view 
		 frustum.

		 @param[in]		point
						A reference to the point.
		 @param[in]		epsilon
						The epsilon value for F32 comparisons.
		 @return		The coverage of @a point with regard to this view 
						frustum.
		 */
		Coverage Classify(const Point3 &point, 
			F32 epsilon = 0.0f) const noexcept;

		/**
		 Classifies the coverage of the given point with regard to this view 
		 frustum.

		 @param[in]		point
						The point.
		 @param[in]		epsilon
						The epsilon value for F32 comparisons.
		 @return		The coverage of @a point with regard to this view 
						frustum.
		 */
		Coverage XM_CALLCONV Classify(FXMVECTOR point,
			F32 epsilon = 0.0f) const noexcept;

		/**
		 Classifies the coverage of the given AABB with regard to this view 
		 frustum.

		 @param[in]		aabb
						A reference to the AABB.
		 @return		The coverage of @a aabb with regard to this view 
						frustum.
		 */
		Coverage Classify(const AABB &aabb) const noexcept;

		/**
		 Classifies the coverage of the given BS with regard to this view 
		 frustum.

		 @param[in]		bs
						A reference to the BS.
		 @return		The coverage of @a bs with regard to this view frustum.
		 */
		Coverage Classify(const BS &bs) const noexcept;

	private:

		//---------------------------------------------------------------------
		// Member Variables
		//---------------------------------------------------------------------

		union {
			
			struct {

				/**
				 The left plane of this view frustum.
				 */
				XMVECTOR m_left_plane;

				/**
				 The right plane of this view frustum.
				 */
				XMVECTOR m_right_plane;

				/**
				 The bottom plane of this view frustum.
				 */
				XMVECTOR m_bottom_plane;

				/**
				 The top plane of this view frustum.
				 */
				XMVECTOR m_top_plane;

				/**
				 The near plane of this view frustum.
				 */
				XMVECTOR m_near_plane;

				/**
				 The far plane of this view frustum.
				 */
				XMVECTOR m_far_plane;
			};
			
			/**
			 The six planes of this view frustum. 
			 */
			XMVECTOR m_planes[6];
		};
	};
}