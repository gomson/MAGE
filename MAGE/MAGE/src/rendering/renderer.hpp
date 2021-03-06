#pragma once

//-----------------------------------------------------------------------------
// Engine Includes
//-----------------------------------------------------------------------------
#pragma region

#include "rendering\pass\aa_pass.hpp"
#include "rendering\pass\back_buffer_pass.hpp"
#include "rendering\pass\bounding_volume_pass.hpp"
#include "rendering\pass\constant_component_pass.hpp"
#include "rendering\pass\constant_shading_pass.hpp"
#include "rendering\pass\deferred_shading_pass.hpp"
#include "rendering\pass\depth_pass.hpp"
#include "rendering\pass\dof_pass.hpp"
#include "rendering\pass\gbuffer_pass.hpp"
#include "rendering\pass\lbuffer_pass.hpp"
#include "rendering\pass\shading_normal_pass.hpp"
#include "rendering\pass\sky_pass.hpp"
#include "rendering\pass\sprite_pass.hpp"
#include "rendering\pass\variable_component_pass.hpp"
#include "rendering\pass\variable_shading_pass.hpp"
#include "rendering\pass\wireframe_pass.hpp"

#include "rendering\buffer\game_buffer.hpp"
#include "rendering\buffer\camera_buffer.hpp"

#pragma endregion

//-----------------------------------------------------------------------------
// Engine Declarations
//-----------------------------------------------------------------------------
namespace mage {

	/**
	 A class of renderers.
	 */
	class Renderer final {

	public:

		//---------------------------------------------------------------------
		// Class Member Variables
		//---------------------------------------------------------------------

		/**
		 Returns the renderer of the rendering manager associated with the 
		 current engine.

		 @pre			The rendering manager associated with the current 
						engine must be loaded.
		 @return		A pointer to the renderer of the rendering manager 
						associated with the current engine.
		 */
		static Renderer *Get() noexcept;

		//---------------------------------------------------------------------
		// Constructors and Destructors
		//---------------------------------------------------------------------

		/**
		 Constructs a renderer.

		 @pre			@a device is not equal to @c nullptr.
		 @pre			@a device_context is not equal to @c nullptr.
		 @param[in]		device
						A pointer to the device.
		 @param[in]		device_context
						A pointer to the device context.
		 @param[in]		width
						The width in pixels of the back buffer.
		 @param[in]		height
						The height in pixels of the back buffer.
		 */
		explicit Renderer(ID3D11Device5 *device, 
			ID3D11DeviceContext4 *device_context, 
			U32 width, U32 height);

		/**
		 Constructs a renderer from the given renderer.

		 @param[in]		renderer
						A reference to the renderer to copy.
		 */
		Renderer(const Renderer &renderer) = delete;

		/**
		 Constructs a renderer by moving the given renderer.

		 @param[in]		renderer
						A reference to the renderer to move.
		 */
		Renderer(Renderer &&renderer);

		/**
		 Destructs this renderer.
		 */
		~Renderer();

		//---------------------------------------------------------------------
		// Assignment Operators
		//---------------------------------------------------------------------

		/**
		 Copies the given renderer to this renderer.

		 @param[in]		renderer
						A reference to the renderer to copy.
		 @return		A reference to the copy of the given renderer (i.e. 
						this renderer).
		 */
		Renderer &operator=(const Renderer &renderer) = delete;

		/**
		 Moves the given renderer to this renderer.

		 @param[in]		renderer
						A reference to the renderer to move.
		 @return		A reference to the moved renderer (i.e. this renderer).
		 */
		Renderer &operator=(Renderer &&renderer) = delete;

		//---------------------------------------------------------------------
		// Member Methods
		//---------------------------------------------------------------------

		/**
		 Binds the persistent state of this renderer.

		 @throws		FormattedException
						Failed to bind the persistent state of this renderer.
		 */
		void BindPersistentState();

		/**
		 Renders the given scene.

		 @pre			@a scene is not equal to @c nullptr.
		 @param[in]		scene
						A pointer to the scene.
		 @throws		FormattedException
						Failed to render the scene.
		 */
		void Render(const Scene *scene);

		//---------------------------------------------------------------------
		// Member Methods: Render Passes
		//---------------------------------------------------------------------

		/**
		 Returns the AA pass of this renderer.

		 @pre			The rendering manager associated with the current 
						engine must be loaded.
		 @pre			The resource manager associated with the current engine 
						must be loaded.
		 @return		A pointer to the AA pass of this renderer.
		 */
		AAPass *GetAAPass() {
			if (!m_aa_pass) {
				m_aa_pass = MakeUnique< AAPass >();
			}
			return m_aa_pass.get();
		}

		/**
		 Returns the back buffer pass of this renderer.

		 @pre			The rendering manager associated with the current 
						engine must be loaded.
		 @pre			The resource manager associated with the current engine 
						must be loaded.
		 @return		A pointer to the back buffer pass of this renderer.
		 */
		BackBufferPass *GetBackBufferPass() {
			if (!m_back_buffer_pass) {
				m_back_buffer_pass = MakeUnique< BackBufferPass >();
			}
			return m_back_buffer_pass.get();
		}

		/**
		 Returns the bounding volume pass of this renderer.

		 @pre			The rendering manager associated with the current 
						engine must be loaded.
		 @pre			The resource manager associated with the current engine 
						must be loaded.
		 @return		A pointer to the bounding volume pass of this scene 
						renderer.
		 */
		BoundingVolumePass *GetBoundingVolumePass() {
			if (!m_bounding_volume_pass) {
				m_bounding_volume_pass = MakeUnique< BoundingVolumePass >();
			}
			return m_bounding_volume_pass.get();
		}

		/**
		 Returns the constant component pass of this renderer.

		 @pre			The rendering manager associated with the current 
						engine must be loaded.
		 @pre			The resource manager associated with the current engine 
						must be loaded.
		 @return		A pointer to the constant component pass of this scene 
						renderer.
		 */
		ConstantComponentPass *GetConstantComponentPass() {
			if (!m_constant_component_pass) {
				m_constant_component_pass = MakeUnique< ConstantComponentPass >();
			}
			return m_constant_component_pass.get();
		}

		/**
		 Returns the constant shading pass of this renderer.

		 @pre			The rendering manager associated with the current 
						engine must be loaded.
		 @pre			The resource manager associated with the current engine 
						must be loaded.
		 @return		A pointer to the constant shading pass of this scene 
						renderer.
		 */
		ConstantShadingPass *GetConstantShadingPass() {
			if (!m_constant_shading_pass) {
				m_constant_shading_pass = MakeUnique< ConstantShadingPass >();
			}
			return m_constant_shading_pass.get();
		}

		/**
		 Returns the deferred shading pass of this renderer.

		 @pre			The rendering manager associated with the current 
						engine must be loaded.
		 @pre			The resource manager associated with the current engine 
						must be loaded.
		 @return		A pointer to the deferred shading pass of this scene 
						renderer.
		 */
		DeferredShadingPass *GetDeferredShadingPass() {
			if (!m_deferred_shading_pass) {
				m_deferred_shading_pass = MakeUnique< DeferredShadingPass >();
			}
			return m_deferred_shading_pass.get();
		}

		/**
		 Returns the depth pass of this renderer.

		 @pre			The rendering manager associated with the current 
						engine must be loaded.
		 @pre			The resource manager associated with the current engine 
						must be loaded.
		 @return		A pointer to the depth pass of this renderer.
		 */
		DepthPass *GetDepthPass() {
			if (!m_depth_pass) {
				m_depth_pass = MakeUnique< DepthPass >();
			}
			return m_depth_pass.get();
		}

		/**
		 Returns the depth-of-field pass of this renderer.

		 @pre			The rendering manager associated with the current 
						engine must be loaded.
		 @pre			The resource manager associated with the current engine 
						must be loaded.
		 @return		A pointer to the depth-of-field pass of this renderer.
		 */
		DOFPass *GetDOFPass() {
			if (!m_dof_pass) {
				m_dof_pass = MakeUnique< DOFPass >();
			}
			return m_dof_pass.get();
		}

		/**
		 Returns the GBuffer pass of this renderer.

		 @pre			The rendering manager associated with the current 
						engine must be loaded.
		 @pre			The resource manager associated with the current engine 
						must be loaded.
		 @return		A pointer to the GBuffer pass of this renderer.
		 */
		GBufferPass *GetGBufferPass() {
			if (!m_gbuffer_pass) {
				m_gbuffer_pass = MakeUnique< GBufferPass >();
			}
			return m_gbuffer_pass.get();
		}

		/**
		 Returns the LBuffer pass of this renderer.

		 @pre			The rendering manager associated with the current 
						engine must be loaded.
		 @pre			The resource manager associated with the current engine 
						must be loaded.
		 @return		A pointer to the LBuffer pass of this renderer.
		 */
		LBufferPass *GetLBufferPass() {
			if (!m_lbuffer_pass) {
				m_lbuffer_pass = MakeUnique< LBufferPass >();
			}
			return m_lbuffer_pass.get();
		}

		/**
		 Returns the shading normal pass of this renderer.

		 @pre			The rendering manager associated with the current 
						engine must be loaded.
		 @pre			The resource manager associated with the current engine 
						must be loaded.
		 @return		A pointer to the shading normal pass of this scene 
						renderer.
		 */
		ShadingNormalPass *GetShadingNormalPass() {
			if (!m_shading_normal_pass) {
				m_shading_normal_pass = MakeUnique< ShadingNormalPass >();
			}
			return m_shading_normal_pass.get();
		}

		/**
		 Returns the sky pass of this renderer.

		 @pre			The rendering manager associated with the current 
						engine must be loaded.
		 @pre			The resource manager associated with the current engine 
						must be loaded.
		 @return		A pointer to the sky pass of this renderer.
		 */
		SkyPass *GetSkyPass() {
			if (!m_sky_pass) {
				m_sky_pass = MakeUnique< SkyPass >();
			}
			return m_sky_pass.get();
		}

		/**
		 Returns the sprite pass of this renderer.

		 @pre			The rendering manager associated with the current 
						engine must be loaded.
		 @pre			The resource manager associated with the current engine 
						must be loaded.
		 @return		A pointer to the sprite pass of this renderer.
		 */
		SpritePass *GetSpritePass() {
			if (!m_sprite_pass) {
				m_sprite_pass = MakeUnique< SpritePass >();
			}
			return m_sprite_pass.get();
		}

		/**
		 Returns the variable component pass of this renderer.

		 @pre			The rendering manager associated with the current 
						engine must be loaded.
		 @pre			The resource manager associated with the current engine 
						must be loaded.
		 @return		A pointer to the variable component pass of this scene 
						renderer.
		 */
		VariableComponentPass *GetVariableComponentPass() {
			if (!m_variable_component_pass) {
				m_variable_component_pass = MakeUnique< VariableComponentPass >();
			}
			return m_variable_component_pass.get();
		}

		/**
		 Returns the variable shading pass of this renderer.

		 @pre			The rendering manager associated with the current 
						engine must be loaded.
		 @pre			The resource manager associated with the current engine 
						must be loaded.
		 @return		A pointer to the variable shading pass of this scene 
						renderer.
		 */
		VariableShadingPass *GetVariableShadingPass() {
			if (!m_variable_shading_pass) {
				m_variable_shading_pass = MakeUnique< VariableShadingPass >();
			}
			return m_variable_shading_pass.get();
		}

		/**
		 Returns the wireframe pass of this renderer.

		 @pre			The rendering manager associated with the current 
						engine must be loaded.
		 @pre			The resource manager associated with the current engine 
						must be loaded.
		 @return		A pointer to the wireframe pass of this renderer.
		 */
		WireframePass *GetWireframePass() {
			if (!m_wireframe_pass) {
				m_wireframe_pass = MakeUnique< WireframePass >();
			}
			return m_wireframe_pass.get();
		}

	private:

		//---------------------------------------------------------------------
		// Member Methods
		//---------------------------------------------------------------------

		/**
		 Binds the camera buffer of this renderer.
		 
		 @pre			@a camera is not equal to @c nullptr.
		 @param[in]		camera
						A pointer to the camera.
		 @param[in]		viewport
						A reference to the viewport.
		 @param[in]		ss_viewport
						A reference to the super-sampled viewport.
		 @param[in]		view_to_projection
						The view-to-projection transformation matrix.
		 @param[in]		projection_to_view
						The projection-to-view transformation matrix.
		 @param[in]		world_to_view
						The world-to-view transformation matrix.
		 @param[in]		view_to_world
						The view-to-world transformation matrix.
		 @throws		FormattedException
						Failed to bind the persistent state of this renderer.
		 */
		void BindCameraBuffer(
			const Camera *camera,
			const Viewport &viewport,
			const Viewport &ss_viewport,
			FXMMATRIX view_to_projection, 
			CXMMATRIX projection_to_view,
			CXMMATRIX world_to_view,
			CXMMATRIX view_to_world);

		void ExecuteSolidForwardPipeline(
			const Viewport &viewport,
			FXMMATRIX world_to_projection,
			CXMMATRIX world_to_view,
			CXMMATRIX view_to_world);

		void ExecuteForwardPipeline(
			const Viewport &viewport,
			FXMMATRIX world_to_projection,
			CXMMATRIX world_to_view,
			CXMMATRIX view_to_world,
			BRDFType brdf);

		void ExecuteDeferredPipeline(
			const Viewport &viewport,
			FXMMATRIX world_to_projection,
			CXMMATRIX world_to_view,
			CXMMATRIX view_to_world,
			BRDFType brdf);

		void ExecuteAAPipeline(
			const Viewport &viewport);

		//---------------------------------------------------------------------
		// Member Variables
		//---------------------------------------------------------------------

		/**
		 A pointer to the device context of this renderer.
		 */
		ID3D11DeviceContext4 * const m_device_context;

		/**
		 The maximum viewport of this renderer.
		 */
		Viewport m_maximum_viewport;

		/**
		 A pointer to the pass buffer of this renderer.
		 */
		UniquePtr< PassBuffer > m_pass_buffer;
		
		/**
		 A pointer to the game buffer of this renderer.
		 */
		ConstantBuffer< GameBuffer > m_game_buffer;

		/**
		 A pointer to the camera buffer of this renderer.
		 */
		ConstantBuffer< CameraBuffer > m_camera_buffer;
		
		//---------------------------------------------------------------------
		// Member Variables: Render Passes
		//---------------------------------------------------------------------

		/**
		 A pointer to the AA pass of this renderer.
		 */
		UniquePtr< AAPass > m_aa_pass;

		/**
		 A pointer to the back buffer pass of this renderer.
		 */
		UniquePtr< BackBufferPass > m_back_buffer_pass;

		/**
		 A pointer to the bounding volume pass of this renderer.
		 */
		UniquePtr< BoundingVolumePass > m_bounding_volume_pass;

		/**
		 A pointer to the constant component pass of this renderer.
		 */
		UniquePtr< ConstantComponentPass > m_constant_component_pass;
		
		/**
		 A pointer to the constant shading pass of this renderer.
		 */
		UniquePtr< ConstantShadingPass > m_constant_shading_pass;

		/**
		 A pointer to the deferred shading pass of this renderer.
		 */
		UniquePtr< DeferredShadingPass > m_deferred_shading_pass;

		/**
		 A pointer to the depth pass of this renderer.
		 */
		UniquePtr< DepthPass > m_depth_pass;

		/**
		 A pointer to the depth-of-field pass of this renderer.
		 */
		UniquePtr< DOFPass > m_dof_pass;

		/**
		 A pointer to the GBuffer pass of this renderer.
		 */
		UniquePtr< GBufferPass >  m_gbuffer_pass;

		/**
		 A pointer to the LBuffer pass of this renderer.
		 */
		UniquePtr< LBufferPass >  m_lbuffer_pass;

		/**
		 A pointer to the shading normal pass of this renderer.
		 */
		UniquePtr< ShadingNormalPass > m_shading_normal_pass;

		/**
		 A pointer to the sky pass of this renderer.
		 */
		UniquePtr< SkyPass > m_sky_pass;

		/**
		 A pointer to the sprite pass of this renderer.
		 */
		UniquePtr< SpritePass > m_sprite_pass;

		/**
		 A pointer to the variable component pass of this renderer.
		 */
		UniquePtr< VariableComponentPass > m_variable_component_pass;

		/**
		 A pointer to the variable shading pass of this renderer.
		 */
		UniquePtr< VariableShadingPass > m_variable_shading_pass;

		/**
		 A pointer to the wireframe pass of this renderer.
		 */
		UniquePtr< WireframePass > m_wireframe_pass;
	};
}