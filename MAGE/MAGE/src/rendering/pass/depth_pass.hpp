#pragma once

//-----------------------------------------------------------------------------
// Engine Includes
//-----------------------------------------------------------------------------
#pragma region

#include "rendering\pass\pass_buffer.hpp"
#include "rendering\buffer\constant_buffer.hpp"
#include "rendering\buffer\model_buffer.hpp"
#include "shader\shader.hpp"

#pragma endregion

//-----------------------------------------------------------------------------
// Engine Declarations end Definitions
//-----------------------------------------------------------------------------
namespace mage {

	/**
	 A class of depth passes for rendering models to a depth buffer only.
	 */
	class DepthPass final {

	public:

		//---------------------------------------------------------------------
		// Class Member Methods
		//---------------------------------------------------------------------

		/**
		 Returns the depth pass associated with the current engine.

		 @pre			The scene renderer associated with the current engine 
						must be loaded.
		 @return		A pointer to the depth pass associated with the current 
						engine.
		 */
		static DepthPass *Get();

		//---------------------------------------------------------------------
		// Constructors and Destructors
		//---------------------------------------------------------------------

		/**
		 Constructs a depth pass.

		 @pre			The renderer associated with the current engine must be 
						loaded.
		 @pre			The resource manager associated with the current engine 
						must be loaded.
		 */
		DepthPass();

		/**
		 Constructs a depth pass from the given depth pass.

		 @param[in]		render_pass
						A reference to the depth pass to copy.
		 */
		DepthPass(const DepthPass &render_pass) = delete;

		/**
		 Constructs a depth pass by moving the given depth pass.

		 @param[in]		render_pass
						A reference to the depth pass to move.
		 */
		DepthPass(DepthPass &&render_pass);

		/**
		 Destructs this depth pass.
		 */
		~DepthPass();

		//---------------------------------------------------------------------
		// Assignment Operators
		//---------------------------------------------------------------------

		/**
		 Copies the given depth pass to this depth pass.

		 @param[in]		render_pass
						A reference to the depth pass to copy.
		 @return		A reference to the copy of the given depth pass (i.e. 
						this depth pass).
		 */
		DepthPass &operator=(const DepthPass &render_pass) = delete;

		/**
		 Moves the given depth pass to this depth pass.

		 @param[in]		render_pass
						A reference to the depth pass to move.
		 @return		A reference to the moved depth pass (i.e. this depth 
						pass).
		 */
		DepthPass &operator=(DepthPass &&render_pass) = delete;

		//---------------------------------------------------------------------
		// Member Methods
		//---------------------------------------------------------------------
		
		/**
		 Binds the fixed state of this depth pass.

		 @throws		FormattedException
						Failed to bind the fixed state of this depth pass.
		 */
		void BindFixedState();

		/**
		 Renders the scene.

		 @pre			@a scene is not equal to @c nullptr.
		 @param[in]		scene
						A pointer to the scene.
		 @param[in]		world_to_projection
						The world-to-projection transformation matrix.
		 @param[in]		world_to_view
						The world-to-view transformation matrix.
		 @param[in]		view_to_projection
						The view-to-projection transformation matrix.
		 @throws		FormattedException
						Failed to render the scene.
		 */
		void XM_CALLCONV Render(
			const PassBuffer *scene,
			FXMMATRIX world_to_projection,
			CXMMATRIX world_to_view,
			CXMMATRIX view_to_projection);

		/**
		 Renders the occluders of the scene.

		 @pre			@a scene is not equal to @c nullptr.
		 @param[in]		scene
						A pointer to the scene.
		 @param[in]		world_to_projection
						The world-to-projection transformation matrix.
		 @param[in]		world_to_view
						The world-to-view transformation matrix.
		 @param[in]		view_to_projection
						The view-to-projection transformation matrix.
		 @throws		FormattedException
						Failed to render the scene.
		 */
		void XM_CALLCONV RenderOccluders(
			const PassBuffer *scene,
			FXMMATRIX world_to_projection,
			CXMMATRIX world_to_view,
			CXMMATRIX view_to_projection);

	private:

		//---------------------------------------------------------------------
		// Member Methods
		//---------------------------------------------------------------------

		/**
		 Binds the shaders for opaque models.
		 */
		void BindOpaqueModelShaders() const noexcept;
		
		/**
		 Binds the shaders for transparent models.
		 */
		void BindTransparentModelShaders() const noexcept;

		/**
		 Binds the projection data of this depth pass.

		 @param[in]		view_to_projection
						The view-to-projection transformation matrix used for
						transforming vertices.
		 @throws		FormattedException
						Failed to bind the projection data of this depth pass.
		 */
		void XM_CALLCONV BindProjectionData(FXMMATRIX view_to_projection);

		/**
		 Binds the opaque model data of this depth pass.

		 @param[in]		object_to_view
						The object-to-view transformation matrix used for
						transforming vertices.
		 @throws		FormattedException
						Failed to bind the model data of this depth pass.
		 */
		void XM_CALLCONV BindOpaqueModelData(FXMMATRIX object_to_view);

		/**
		 Binds the transparent model data of this depth pass.

		 @pre			@a material is not equal to @c nullptr.
		 @param[in]		object_to_view
						The object-to-view transformation matrix used for
						transforming vertices.
		 @param[in]		texture_transform
						The texture transformation matrix used for transforming 
						texture coordinates.
		 @param[in]		material
						A pointer to the material.
		 @throws		FormattedException
						Failed to bind the model data of this depth pass.
		 */
		void XM_CALLCONV BindTransparentModelData(
			FXMMATRIX object_to_view,
			CXMMATRIX texture_transform,
			const Material *material);

		/**
		 Process the given opaque models.

		 @param[in]		models
						A reference to a vector containing the model pointers
						to process.
		 @param[in]		world_to_projection
						The world-to-projection transformation matrix. This 
						transformation matrix will be used for culling.
		 @param[in]		world_to_view
						The world-to-view transformation matrix. This 
						transformation matrix will be chained with the 
						object-to-view transformation matrix for transforming 
						vertices.
		 @throws		FormattedException
						Failed to process the models.
		 */
		void XM_CALLCONV ProcessOpaqueModels(
			const vector< const ModelNode * > &models,
			FXMMATRIX world_to_projection, 
			CXMMATRIX world_to_view);

		/**
		 Process the given opaque occluder models.

		 @param[in]		models
						A reference to a vector containing the model pointers
						to process.
		 @param[in]		world_to_projection
						The world-to-projection transformation matrix.
						This transformation matrix will be used for 
						culling.
		 @param[in]		world_to_view
						The world-to-view transformation matrix. This 
						transformation matrix will be chained with the 
						object-to-view transformation matrix for transforming 
						vertices.
		 @throws		FormattedException
						Failed to process the models.
		 */
		void XM_CALLCONV ProcessOpaqueOccluderModels(
			const vector< const ModelNode * > &models,
			FXMMATRIX world_to_projection, 
			CXMMATRIX world_to_view);

		/**
		 Process the given transparent occluder models.

		 @param[in]		models
						A reference to a vector containing the model pointers
						to process.
		 @param[in]		world_to_projection
						The world-to-projection transformation matrix.
						This transformation matrix will be used for 
						culling.
		 @param[in]		world_to_view
						The world-to-view transformation matrix. This 
						transformation matrix will be chained with the 
						object-to-view transformation matrix for transforming 
						vertices.
		 @throws		FormattedException
						Failed to process the models.
		 */
		void XM_CALLCONV ProcessTransparentOccluderModels(
			const vector< const ModelNode * > &models,
			FXMMATRIX world_to_projection, 
			CXMMATRIX world_to_view);

		//---------------------------------------------------------------------
		// Member Variables
		//---------------------------------------------------------------------

		/**
		 A pointer to the device context of this depth pass.
		 */
		ID3D11DeviceContext4 * const m_device_context;

		/**
		 A pointer to the vertex shader of this depth pass.
		 */
		const SharedPtr< const VertexShader > m_opaque_vs;

		/**
		 A pointer to the vertex shader for transparent models
		 of this depth pass.
		 */
		const SharedPtr< const VertexShader > m_transparent_vs;

		/**
		 A pointer to the pixel shader for transparent models
		 of this depth pass.
		 */
		const SharedPtr< const PixelShader > m_transparent_ps;

		/**
		 The projection buffer of this depth pass.
		 */
		ConstantBuffer< XMMATRIX > m_projection_buffer;

		/**
		 The opaque model buffer of this depth pass.
		 */
		ConstantBuffer< XMMATRIX > m_opaque_model_buffer;

		/**
		 The transparent model buffer of this depth pass.
		 */
		ConstantBuffer< ModelTextureTransformBuffer > m_transparent_model_buffer;

		/**
		 The dissolve buffer of this depth pass. 
		 */
		ConstantBuffer< XMVECTOR > m_dissolve_buffer;
	};
}