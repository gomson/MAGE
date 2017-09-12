//-----------------------------------------------------------------------------
// Engine Includes
//-----------------------------------------------------------------------------
#pragma region

#include "scene\scene_renderer.hpp"
#include "rendering\renderer.hpp"

#pragma endregion

//-----------------------------------------------------------------------------
// Engine Definitions
//-----------------------------------------------------------------------------
namespace mage {

	SceneRenderer::SceneRenderer()
		: m_device_context(GetImmediateDeviceContext()),
		m_pass_buffer(MakeUnique< PassBuffer >()),
		m_gbuffer(MakeUnique< GBuffer >()),
		m_lbuffer(MakeUnique< LBuffer >()),
		m_depth_pass(MakeUnique< DepthPass >()),
		m_gbuffer_pass(MakeUnique< GBufferPass >()),
		m_deferred_shading_pass(MakeUnique< DeferredShadingPass >()),
		m_variable_shading_pass(MakeUnique< VariableShadingPass >()),
		m_sprite_pass(MakeUnique< SpritePass >()),
		m_image_pass(MakeUnique< ImagePass >()),
		m_constant_shading_pass(MakeUnique< ConstantShadingPass >()),
		m_constant_component_pass(MakeUnique< ConstantComponentPass >()),
		m_variable_component_pass(MakeUnique< VariableComponentPass >()),
		m_shading_normal_pass(MakeUnique< ShadingNormalPass >()),
		m_wireframe_pass(MakeUnique< WireframePass >()),
		m_bounding_volume_pass(MakeUnique< BoundingVolumePass >()),
		m_viewport() {}
	
	SceneRenderer::SceneRenderer(SceneRenderer &&scene_renderer) = default;
	
	SceneRenderer::~SceneRenderer() = default;

	void SceneRenderer::Render(const Scene *scene) {
		const Renderer * const renderer = Renderer::Get();
		
		// Update the pass buffer.
		m_pass_buffer->Update(scene);
		
		for (const auto node : m_pass_buffer->m_cameras) {
			
			// Obtain node components.
			const TransformNode  * const transform = node->GetTransform();
			const Camera         * const camera    = node->GetCamera();
			const XMMATRIX world_to_view           = transform->GetWorldToViewMatrix();
			const XMMATRIX view_to_world           = transform->GetViewToWorldMatrix();
			const XMMATRIX view_to_projection      = camera->GetViewToProjectionMatrix();
			const XMMATRIX world_to_projection     = world_to_view * view_to_projection;
			const CameraSettings * const settings  = node->GetSettings();
			const RenderMode render_mode           = settings->GetRenderMode();
			const BRDFType brdf                    = settings->GetBRDF();
			const Viewport &viewport               = node->GetViewport();

			// Bind the viewport.
			viewport.BindViewport(m_device_context);

			// RenderMode
			switch (render_mode) {

			case RenderMode::DepthAndForward: {
				OM::BindRTVAndDSV(m_device_context, nullptr, renderer->GetDepthBufferDSV());
				m_depth_pass->BindFixedState();
				m_depth_pass->Render(
					m_pass_buffer.get(), world_to_projection,
					world_to_view, view_to_projection);
				renderer->BindRTVAndDSV();
				// Fall through RenderMode::Forward.
			}
			case RenderMode::Forward: {
				m_lbuffer->Render(
					m_pass_buffer.get(), world_to_projection,
					world_to_view, view_to_world);
				
				m_lbuffer->BindToGraphicsPipeline();
				m_variable_shading_pass->BindFixedState(brdf);
				m_variable_shading_pass->Render(
					m_pass_buffer.get(), world_to_projection,
					world_to_view, view_to_world, view_to_projection);
				
				break;
			}

			case RenderMode::Deferred: {
				m_gbuffer->BindPacking(m_device_context);
				m_gbuffer_pass->BindFixedState();
				m_gbuffer_pass->Render(
					m_pass_buffer.get(), world_to_projection,
					world_to_view, view_to_world, view_to_projection);
				
				m_gbuffer->BindUnpacking(m_device_context);
				m_lbuffer->Render(
					m_pass_buffer.get(), world_to_projection,
					world_to_view, view_to_world);
				
				m_lbuffer->BindToComputePipeline();
				m_deferred_shading_pass->BindFixedState(brdf);
				m_deferred_shading_pass->Render(
					m_pass_buffer.get(), view_to_projection);

				m_gbuffer->BindRestore(m_device_context);
				m_image_pass->BindFixedState();
				m_image_pass->Render();

				m_lbuffer->BindToGraphicsPipeline();
				m_variable_shading_pass->BindFixedState(brdf);
				m_variable_shading_pass->RenderPostDeferred(
					m_pass_buffer.get(), world_to_projection,
					world_to_view, view_to_world, view_to_projection);
				
				break;
			}

			case RenderMode::DepthAndSolid: {
				OM::BindRTVAndDSV(m_device_context, nullptr, renderer->GetDepthBufferDSV());
				m_depth_pass->BindFixedState();
				m_depth_pass->Render(
					m_pass_buffer.get(), world_to_projection, 
					world_to_view, view_to_projection);
				renderer->BindRTVAndDSV();
				// Fall through RenderMode::Solid.
			}
			case RenderMode::Solid: {
				m_lbuffer->Render(
					m_pass_buffer.get(), world_to_projection,
					world_to_view, view_to_world);
				
				m_lbuffer->BindToGraphicsPipeline();
				m_constant_shading_pass->BindFixedState();
				m_constant_shading_pass->Render(
					m_pass_buffer.get(), world_to_projection, 
					world_to_view, view_to_world, view_to_projection);
				
				break;
			}

			case RenderMode::DiffuseColor:
			case RenderMode::DiffuseReflectivity:
			case RenderMode::DiffuseReflectivityTexture:
			case RenderMode::SpecularColor:
			case RenderMode::SpecularReflectivity:
			case RenderMode::SpecularReflectivityTexture:
			case RenderMode::NormalTexture: {
				m_variable_component_pass->BindFixedState(render_mode);
				m_variable_component_pass->Render(
					m_pass_buffer.get(), world_to_projection, 
					world_to_view, view_to_world, view_to_projection);
				break;
			}

			case RenderMode::UVTexture:
			case RenderMode::Distance: {
				m_constant_component_pass->BindFixedState(render_mode);
				m_constant_component_pass->Render(
					m_pass_buffer.get(), world_to_projection, 
					world_to_view, view_to_world, view_to_projection);
				break;
			}

			case RenderMode::ShadingNormal:
			case RenderMode::TSNMShadingNormal: {
				m_shading_normal_pass->BindFixedState(render_mode);
				m_shading_normal_pass->Render(
					m_pass_buffer.get(), world_to_projection, 
					world_to_view, view_to_projection);
				break;
			}

			}

			// RenderLayer
			if (settings->HasRenderLayer(RenderLayer::Wireframe)) {
				m_wireframe_pass->BindFixedState();
				m_wireframe_pass->Render(
					m_pass_buffer.get(), world_to_projection, 
					world_to_view, view_to_projection);
			}
			if (settings->HasRenderLayer(RenderLayer::AABB)) {
				m_bounding_volume_pass->BindFixedState();
				m_bounding_volume_pass->Render(
					m_pass_buffer.get(), world_to_projection);
			}
		}

		// Bind the maximum viewport.
		m_viewport.BindViewport(m_device_context);
		
		m_sprite_pass->Render(m_pass_buffer.get());
	}
}