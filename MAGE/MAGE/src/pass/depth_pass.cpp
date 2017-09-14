//-----------------------------------------------------------------------------
// Engine Includes
//-----------------------------------------------------------------------------
#pragma region

#include "scene\scene_renderer.hpp"
#include "rendering\rendering_state_cache.hpp"
#include "resource\resource_factory.hpp"
#include "math\view_frustum.hpp"
#include "logging\error.hpp"

// Include HLSL bindings.
#include "..\..\shaders\hlsl.hpp"

#pragma endregion

//-----------------------------------------------------------------------------
// Engine Definitions
//-----------------------------------------------------------------------------
namespace mage {

	DepthPass *DepthPass::Get() {
		Assert(SceneRenderer::Get());

		return SceneRenderer::Get()->GetDepthPass();
	}

	DepthPass::DepthPass()
		: m_device_context(GetImmediateDeviceContext()),
		m_vs(CreateDepthVS()),
		m_transparent_vs(CreateDepthTransparentVS()),
		m_transparent_ps(CreateDepthTransparentPS()),
		m_projection_buffer(), m_model_buffer() {}

	DepthPass::DepthPass(DepthPass &&render_pass) = default;

	DepthPass::~DepthPass() = default;

	void XM_CALLCONV DepthPass::BindProjectionData(
		FXMMATRIX view_to_projection) {

		// Update the projection buffer.
		m_projection_buffer.UpdateData(m_device_context, 
			XMMatrixTranspose(view_to_projection));
		// Bind the projection buffer.
		VS::BindConstantBuffer(m_device_context,
			SLOT_CBUFFER_PER_FRAME, m_projection_buffer.Get());
	}

	void XM_CALLCONV DepthPass::BindModelData(
		FXMMATRIX object_to_view) {

		// Update the model buffer.
		m_model_buffer.UpdateData(m_device_context, 
			XMMatrixTranspose(object_to_view));
		// Bind the model buffer.
		VS::BindConstantBuffer(m_device_context,
			SLOT_CBUFFER_PER_DRAW, m_model_buffer.Get());
	}

	void DepthPass::BindFixedState() {
		// Bind the vertex shader.
		m_vs->BindShader(m_device_context);
		// Bind no pixel shader.
		PS::BindShader(m_device_context, nullptr);
		// Bind the rasterization state.
		RenderingStateCache::Get()->BindCullCounterClockwiseRasterizerState(m_device_context);
		// Bind the depth-stencil state.
		RenderingStateCache::Get()->BindDepthDefaultDepthStencilState(m_device_context);
	}

	void XM_CALLCONV DepthPass::Render(
		const PassBuffer *scene,
		FXMMATRIX world_to_projection,
		CXMMATRIX world_to_view,
		CXMMATRIX view_to_projection) {

		Assert(scene);

		// Bind the projection data.
		BindProjectionData(view_to_projection);

		// Process the opaque models.
		ProcessModels(scene->GetOpaqueEmissiveModels(), 
			world_to_projection, world_to_view);
		ProcessModels(scene->GetOpaqueBRDFModels(), 
			world_to_projection, world_to_view);
	}

	void XM_CALLCONV DepthPass::RenderOccluders(
		const PassBuffer *scene,
		FXMMATRIX world_to_projection,
		CXMMATRIX world_to_view,
		CXMMATRIX view_to_projection) {

		Assert(scene);

		// Bind the projection data.
		BindProjectionData(view_to_projection);

		// Process the opaque models.
		ProcessModels(scene->GetOpaqueEmissiveModels(),
			world_to_projection, world_to_view);
		ProcessModels(scene->GetOpaqueBRDFModels(),
			world_to_projection, world_to_view);
	}

	void XM_CALLCONV DepthPass::ProcessModels(
		const vector< const ModelNode * > &models,
		FXMMATRIX world_to_projection,
		CXMMATRIX world_to_view) {

		for (const auto node : models) {
			
			// Obtain node components (1/2).
			const TransformNode * const transform = node->GetTransform();
			const Model         * const model     = node->GetModel();
			const XMMATRIX object_to_world        = transform->GetObjectToWorldMatrix();
			const XMMATRIX object_to_projection   = object_to_world * world_to_projection;

			// Cull the model against the view frustum.
			if (ViewFrustum::Cull(object_to_projection, model->GetAABB())) {
				continue;
			}

			// Obtain node components (2/2).
			const XMMATRIX object_to_view         = object_to_world * world_to_view;

			// Bind the model data.
			BindModelData(object_to_view);
			// Bind the model mesh.
			model->BindMesh(m_device_context);
			// Draw the model.
			model->Draw(m_device_context);
		}
	}

	void XM_CALLCONV DepthPass::ProcessOpaqueOccluderModels(
		const vector< const ModelNode * > &models,
		FXMMATRIX world_to_projection,
		CXMMATRIX world_to_view) {

		for (const auto node : models) {

			// Obtain node components (1/3).
			const Model         * const model     = node->GetModel();

			// Skip non-occluder models.
			if (!model->OccludesLight()) {
				continue;
			}

			// Obtain node components (2/3).
			const TransformNode * const transform = node->GetTransform();
			const XMMATRIX object_to_world        = transform->GetObjectToWorldMatrix();
			const XMMATRIX object_to_projection   = object_to_world * world_to_projection;

			// Cull the model against the view frustum.
			if (ViewFrustum::Cull(object_to_projection, model->GetAABB())) {
				continue;
			}

			// Obtain node components (3/3).
			const XMMATRIX object_to_view         = object_to_world * world_to_view;

			// Bind the model data.
			BindModelData(object_to_view);
			// Bind the model mesh.
			model->BindMesh(m_device_context);
			// Draw the model.
			model->Draw(m_device_context);
		}
	}
}