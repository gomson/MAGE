//-----------------------------------------------------------------------------
// Engine Includes
//-----------------------------------------------------------------------------
#pragma region

#include "rendering\rendering_manager.hpp"
#include "resource\resource_factory.hpp"
#include "math\geometry\view_frustum.hpp"
#include "utils\logging\error.hpp"

// Include HLSL bindings.
#include "..\..\..\shaders\hlsl.hpp"

#pragma endregion

//-----------------------------------------------------------------------------
// Engine Definitions
//-----------------------------------------------------------------------------
namespace mage {

	VariableComponentPass *VariableComponentPass::Get() {
		Assert(Renderer::Get());

		return Renderer::Get()->GetVariableComponentPass();
	}

	VariableComponentPass::VariableComponentPass()
		: m_device_context(Pipeline::GetImmediateDeviceContext()), 
		m_vs(CreateMinimalTransformVS()),
		m_ps(CreateConstantColorTexturePS()),
		m_color_buffer(), 
		m_model_buffer(),
		m_white(CreateWhiteTexture()) {}

	VariableComponentPass::VariableComponentPass(
		VariableComponentPass &&render_pass) = default;

	VariableComponentPass::~VariableComponentPass() = default;

	void XM_CALLCONV VariableComponentPass::BindModelData(
		FXMMATRIX object_to_view,
		CXMMATRIX view_to_object,
		CXMMATRIX texture_transform,
		const Material *material) {

		ModelNormalTextureTransformBuffer buffer;
		buffer.m_object_to_view = XMMatrixTranspose(object_to_view);
		buffer.m_normal_to_view = view_to_object;

		switch (m_render_mode) {

		case RenderMode::BaseColor: {

			buffer.m_texture_transform = texture_transform;

			// Update the color buffer.
			m_color_buffer.UpdateData(m_device_context, 
				RGBA(material->GetBaseColor()));
			// Bind the color buffer.
			m_color_buffer.Bind< Pipeline::PS >(
				m_device_context, SLOT_CBUFFER_COLOR);

			// Bind the diffuse SRV.
			Pipeline::PS::BindSRV(m_device_context,
				SLOT_SRV_TEXTURE, material->GetBaseColorSRV());

			break;
		}
		
		case RenderMode::BaseColorCoefficient: {

			buffer.m_texture_transform = texture_transform;

			// Update the color buffer.
			m_color_buffer.UpdateData(m_device_context, 
				RGBA(material->GetBaseColor()));
			// Bind the color buffer.
			m_color_buffer.Bind< Pipeline::PS >(
				m_device_context, SLOT_CBUFFER_COLOR);

			// Bind the diffuse SRV.
			Pipeline::PS::BindSRV(m_device_context,
				SLOT_SRV_TEXTURE, m_white->Get());

			break;
		}

		case RenderMode::BaseColorTexture: {

			buffer.m_texture_transform = texture_transform;

			// Update the color buffer.
			m_color_buffer.UpdateData(m_device_context, 
				RGBA(1.0f));
			// Bind the color buffer.
			m_color_buffer.Bind< Pipeline::PS >(
				m_device_context, SLOT_CBUFFER_COLOR);

			// Bind the diffuse SRV.
			Pipeline::PS::BindSRV(m_device_context,
				SLOT_SRV_TEXTURE, material->GetBaseColorSRV());

			break;
		}

		case RenderMode::Material: {

			buffer.m_texture_transform = texture_transform;

			// Update the color buffer.
			m_color_buffer.UpdateData(m_device_context, 
				material->GetMaterialRGBA());
			// Bind the color buffer.
			m_color_buffer.Bind< Pipeline::PS >(
				m_device_context, SLOT_CBUFFER_COLOR);

			// Bind the diffuse SRV.
			Pipeline::PS::BindSRV(m_device_context,
				SLOT_SRV_TEXTURE, material->GetMaterialSRV());

			break;
		}

		case RenderMode::MaterialCoefficient: {

			buffer.m_texture_transform = texture_transform;

			// Update the color buffer.
			m_color_buffer.UpdateData(m_device_context, 
				material->GetMaterialRGBA());
			// Bind the color buffer.
			m_color_buffer.Bind< Pipeline::PS >(
				m_device_context, SLOT_CBUFFER_COLOR);

			// Bind the diffuse SRV.
			Pipeline::PS::BindSRV(m_device_context,
				SLOT_SRV_TEXTURE, m_white->Get());

			break;
		}

		case RenderMode::MaterialTexture: {

			buffer.m_texture_transform = texture_transform;

			// Update the color buffer.
			m_color_buffer.UpdateData(m_device_context, 
				RGBA(1.0f));
			// Bind the color buffer.
			m_color_buffer.Bind< Pipeline::PS >(
				m_device_context, SLOT_CBUFFER_COLOR);

			// Bind the diffuse SRV.
			Pipeline::PS::BindSRV(m_device_context,
				SLOT_SRV_TEXTURE, material->GetMaterialSRV());

			break;
		}

		case RenderMode::NormalTexture: {

			buffer.m_texture_transform = XMMatrixIdentity();

			// Update the color buffer.
			m_color_buffer.UpdateData(m_device_context, 
				RGBA(1.0f));
			// Bind the color buffer.
			m_color_buffer.Bind< Pipeline::PS >(
				m_device_context, SLOT_CBUFFER_COLOR);

			// Bind the diffuse SRV.
			Pipeline::PS::BindSRV(m_device_context,
				SLOT_SRV_TEXTURE, material->GetNormalSRV());

			break;
		}
		}

		// Update the model buffer.
		m_model_buffer.UpdateData(m_device_context, buffer);
		// Bind the model buffer.
		m_model_buffer.Bind< Pipeline::VS >(
			m_device_context, SLOT_CBUFFER_MODEL);
	}

	void VariableComponentPass::BindFixedState(RenderMode render_mode) {
		// Reset the render mode.
		m_render_mode = render_mode;

		// VS: Bind the vertex shader.
		m_vs->BindShader(m_device_context);
		// HS: Bind the hull shader.
		Pipeline::HS::BindShader(m_device_context, nullptr);
		// DS: Bind the domain shader.
		Pipeline::DS::BindShader(m_device_context, nullptr);
		// GS: Bind the geometry shader.
		Pipeline::GS::BindShader(m_device_context, nullptr);
		// RS: Bind the rasterization state.
		RenderingStateManager::Get()->BindCullCounterClockwiseRasterizerState(m_device_context);
		// PS: Bind the pixel shader.
		m_ps->BindShader(m_device_context);
		// OM: Bind the depth-stencil state.
#ifdef DISSABLE_INVERTED_Z_BUFFER
		RenderingStateManager::Get()->BindLessEqualDepthReadWriteDepthStencilState(m_device_context);
#else  // DISSABLE_INVERTED_Z_BUFFER
		RenderingStateManager::Get()->BindGreaterEqualDepthReadWriteDepthStencilState(m_device_context);
#endif // DISSABLE_INVERTED_Z_BUFFER
		// OM: Bind the blend state.
		RenderingStateManager::Get()->BindOpaqueBlendState(m_device_context);
	}

	void XM_CALLCONV VariableComponentPass::Render(
		const PassBuffer *scene,
		FXMMATRIX world_to_projection,
		CXMMATRIX world_to_view,
		CXMMATRIX view_to_world) {

		Assert(scene);

		// Process the models.
		ProcessModels(scene->GetOpaqueEmissiveModels(), 
			world_to_projection, world_to_view, view_to_world);
		ProcessModels(scene->GetTransparentEmissiveModels(),
			world_to_projection, world_to_view, view_to_world);
		ProcessModels(scene->GetOpaqueBRDFModels(), 
			world_to_projection, world_to_view, view_to_world);
		ProcessModels(scene->GetTransparentBRDFModels(), 
			world_to_projection, world_to_view, view_to_world);
	}

	void XM_CALLCONV VariableComponentPass::ProcessModels(
		const vector< const ModelNode * > &models,
		FXMMATRIX world_to_projection, 
		CXMMATRIX world_to_view, 
		CXMMATRIX view_to_world) {

		for (const auto node : models) {

			// Obtain node components (1/2).
			const TransformNode * const transform = node->GetTransform();
			const Model         * const model     = node->GetModel();
			const XMMATRIX object_to_world        = transform->GetObjectToWorldMatrix();
			const XMMATRIX object_to_projection   = object_to_world * world_to_projection;

			// Apply view frustum culling.
			if (ViewFrustum::Cull(object_to_projection, model->GetAABB())) {
				continue;
			}

			// Obtain node components (2/2).
			const XMMATRIX object_to_view         = object_to_world * world_to_view;
			const XMMATRIX world_to_object        = transform->GetWorldToObjectMatrix();
			const XMMATRIX view_to_object         = view_to_world * world_to_object;
			const XMMATRIX texture_transform      = node->GetTextureTransform()->GetTransformMatrix();
			const Material * const material       = model->GetMaterial();

			// Bind the model data (inc. PS).
			BindModelData(object_to_view, view_to_object, texture_transform, material);
			// Bind the model mesh.
			model->BindMesh(m_device_context);
			// Draw the model.
			model->Draw(m_device_context);
		}
	}
}