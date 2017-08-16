//-----------------------------------------------------------------------------
// Engine Includes
//-----------------------------------------------------------------------------
#pragma region

#include "scene\scene_renderer.hpp"
#include "rendering\rendering_state_cache.hpp"
#include "resource\resource_factory.hpp"
#include "math\view_frustum.hpp"

#pragma endregion

//-----------------------------------------------------------------------------
// Engine Definitions
//-----------------------------------------------------------------------------
namespace mage {

	SceneRenderer::SceneRenderer()
		: m_cameras(), m_opaque_models(), m_transparent_models(), 
		m_directional_lights(), m_omni_lights(), m_spot_lights(), 
		m_sprites(), m_ambient_light(), m_box(),
		m_sprite_batch(MakeUnique< SpriteBatch >()),
		m_transform_buffer(), m_scene_buffer(),
		m_directional_lights_buffer(3),
		m_omni_lights_buffer(64),
		m_spot_lights_buffer(64) {
		
		CreateBox();
	}
	
	SceneRenderer::SceneRenderer(SceneRenderer &&scene_renderer) = default;
	
	SceneRenderer::~SceneRenderer() = default;

	void SceneRenderer::CreateBox() {
		Material box_material("_mat_aabb");
		SharedPtr< Texture > white = CreateWhiteTexture();
		box_material.SetDiffuseReflectivityTexture(white);
		box_material.SetDiffuseReflectivity(RGBSpectrum(0.0f, 1.0f, 0.0f));
		const ShadedMaterial box_shaded_material(box_material, BRDFType::Emissive);

		SharedPtr< const StaticMesh > box_mesh = CreateLineCube();
		m_box = MakeUnique< ModelNode >("_mdl_aabb", box_mesh, box_shaded_material);
	}

	void SceneRenderer::Render(const Scene *scene) {
		Preprocess(scene);
		
		ID3D11SamplerState *sampler_state = RenderingStateCache::Get()->GetLinearWrapSamplerState();
		GetRenderingDeviceContext()->PSSetSamplers(0, 1, &sampler_state);
		
		Render3D(scene);
		//RenderAABBs();
		Render2D();
	}

	void SceneRenderer::Render3D(const Scene *scene) {
		
		ID3D11DepthStencilState *depth_stencil_state = RenderingStateCache::Get()->GetDepthDefaultDepthStencilState();
		GetRenderingDeviceContext()->OMSetDepthStencilState(depth_stencil_state, 0);

		for (auto cit = m_cameras.cbegin(); cit != m_cameras.cend(); ++cit) {
			const CameraNode    * const camera_node      = *cit;
			const TransformNode * const camera_transform = camera_node->GetTransform();
			const Camera        * const camera           = camera_node->GetCamera();

			// Sets the viewport of this camera.
			const D3D11_VIEWPORT &viewport = camera_node->GetViewport();
			GetRenderingDeviceContext()->RSSetViewports(1, &viewport);

			const XMMATRIX world_to_view           = camera_transform->GetWorldToViewMatrix();
			const XMMATRIX view_to_world           = camera_transform->GetViewToWorldMatrix();
			const XMMATRIX view_to_projection      = camera->GetViewToProjectionMatrix();
			const XMMATRIX world_to_projection     = world_to_view * view_to_projection;

			// Update directional light structured buffer.
			vector< DirectionalLightBuffer > directional_lights_buffer;
			directional_lights_buffer.reserve(m_directional_lights.size());
			for (auto it = m_directional_lights.cbegin(); it != m_directional_lights.cend(); ++it) {
				const DirectionalLightNode * const light_node      = *it;
				const TransformNode        * const light_transform = light_node->GetTransform();
				const DirectionalLight     * const light           = light_node->GetLight();

				// Transform to view space.
				const XMVECTOR direction = XMVector3TransformNormal(light_transform->GetWorldForward(), world_to_view);

				// Create a directional light buffer.
				DirectionalLightBuffer light_buffer;
				XMStoreFloat3(&light_buffer.m_d, direction);
				light_buffer.m_I                      = light->GetIntensity();

				// Add directional light buffer to directional light buffers.
				directional_lights_buffer.push_back(std::move(light_buffer));
			}
			m_directional_lights_buffer.UpdateData(directional_lights_buffer);

			// Update omni light structured buffer.
			vector< OmniLightBuffer > omni_lights_buffer;
			omni_lights_buffer.reserve(m_omni_lights.size());
			for (auto it = m_omni_lights.cbegin(); it != m_omni_lights.cend(); ++it) {
				const OmniLightNode        * const light_node      = *it;
				const TransformNode        * const light_transform = light_node->GetTransform();
				const OmniLight            * const light           = light_node->GetLight();

				const XMMATRIX object_to_world      = light_transform->GetObjectToWorldMatrix();
				const XMMATRIX object_to_projection = object_to_world * world_to_projection;

				// Cull the light against the view frustum.
				const ViewFrustum view_frustum(object_to_projection);
				const BS &bs = light->GetBS();
				if (!view_frustum.Overlaps(bs)) {
					continue;
				}

				// Transform to view space.
				const XMVECTOR position  = XMVector3TransformCoord( light_transform->GetWorldEye(),     world_to_view);
			
				// Create a omni light buffer.
				OmniLightBuffer light_buffer;
				XMStoreFloat3(&light_buffer.m_p, position);
				light_buffer.m_I                      = light->GetIntensity();
				light_buffer.m_distance_falloff_end   = light->GetEndDistanceFalloff();
				light_buffer.m_distance_falloff_range = light->GetRangeDistanceFalloff();

				// Add omni light buffer to omni light buffers.
				omni_lights_buffer.push_back(std::move(light_buffer));
			}
			m_omni_lights_buffer.UpdateData(omni_lights_buffer);

			// Update spotlight structured buffer.
			vector< SpotLightBuffer > spot_lights_buffer;
			spot_lights_buffer.reserve(m_spot_lights.size());
			for (auto it = m_spot_lights.cbegin(); it != m_spot_lights.cend(); ++it) {
				const SpotLightNode        * const light_node      = *it;
				const TransformNode        * const light_transform = light_node->GetTransform();
				const SpotLight            * const light           = light_node->GetLight();

				const XMMATRIX object_to_world      = light_transform->GetObjectToWorldMatrix();
				const XMMATRIX object_to_projection = object_to_world * world_to_projection;

				// Cull the light against the view frustum.
				const ViewFrustum view_frustum(object_to_projection);
				const AABB &aabb = light->GetAABB();
				if (!view_frustum.Overlaps(aabb)) {
					continue;
				}

				// Transform to view space.
				const XMVECTOR position  = XMVector3TransformCoord( light_transform->GetWorldEye(),     world_to_view);
				const XMVECTOR direction = XMVector3TransformNormal(light_transform->GetWorldForward(), world_to_view);

				// Create a spotlight buffer.
				SpotLightBuffer light_buffer;
				XMStoreFloat3(&light_buffer.m_p, position);
				XMStoreFloat3(&light_buffer.m_d, direction);
				light_buffer.m_I                      = light->GetIntensity();
				light_buffer.m_exponent_property      = light->GetExponentProperty();
				light_buffer.m_distance_falloff_end   = light->GetEndDistanceFalloff();
				light_buffer.m_distance_falloff_range = light->GetRangeDistanceFalloff();
				light_buffer.m_cos_umbra              = light->GetEndAngularCutoff();
				light_buffer.m_cos_range              = light->GetRangeAngularCutoff();

				// Add spotlight buffer to spotlight buffers.
				spot_lights_buffer.push_back(std::move(light_buffer));
			}
			m_spot_lights_buffer.UpdateData(spot_lights_buffer);

			// Update scene constant buffer.
			SceneBuffer scene_buffer;
			scene_buffer.m_Ia                         = m_ambient_light;
			scene_buffer.m_nb_directional_lights      = static_cast< uint32_t >(directional_lights_buffer.size());
			scene_buffer.m_nb_omni_lights             = static_cast< uint32_t >(omni_lights_buffer.size());
			scene_buffer.m_nb_spot_lights             = static_cast< uint32_t >(spot_lights_buffer.size());
			const SceneFog *fog = scene->GetSceneFog();
			scene_buffer.m_fog_color                  = fog->GetIntensity();
			scene_buffer.m_fog_distance_falloff_start = fog->GetStartDistanceFalloff();
			scene_buffer.m_fog_distance_falloff_range = fog->GetRangeDistanceFalloff();
			m_scene_buffer.UpdateData(scene_buffer);

			// Create lighting buffer.
			SceneInfo scene_info;
			scene_info.m_scene_buffer       = m_scene_buffer.Get();
			scene_info.m_directional_lights = m_directional_lights_buffer.Get();
			scene_info.m_omni_lights        = m_omni_lights_buffer.Get();
			scene_info.m_spot_lights        = m_spot_lights_buffer.Get();

			// Create Transform buffer.
			TransformBuffer transform_buffer(world_to_view, view_to_projection);

			ID3D11BlendState *opaque_blend_state = RenderingStateCache::Get()->GetOpaqueBlendState();
			GetRenderingDeviceContext()->OMSetBlendState(opaque_blend_state, nullptr, 0xFFFFFFFF);

			for (auto it = m_opaque_models.cbegin(); it != m_opaque_models.cend(); ++it) {
				const ModelNode            * const model_node      = *it;
				const TransformNode        * const model_transform = model_node->GetTransform();
				const Model                * const model           = model_node->GetModel();

				const XMMATRIX object_to_world      = model_transform->GetObjectToWorldMatrix();
				const XMMATRIX object_to_projection = object_to_world * world_to_projection;

				// Cull the model against the view frustum.
				const ViewFrustum view_frustum(object_to_projection);
				const AABB &aabb = model->GetAABB();
				if (!view_frustum.Overlaps(aabb)) {
					continue;
				}

				const XMMATRIX world_to_object      = model_transform->GetWorldToObjectMatrix();
				const XMMATRIX view_to_object       = view_to_world * world_to_object;
				
				// Update transform constant buffer.
				transform_buffer.SetObjectMatrices(object_to_world, view_to_object);
				m_transform_buffer.UpdateData(transform_buffer);

				// Draw model.
				model->PrepareDrawing();
				model->PrepareShading(m_transform_buffer.Get(), scene_info);
				model->Draw();
			}

			ID3D11BlendState *alpha_blend_state = RenderingStateCache::Get()->GetAlphaBlendState();
			GetRenderingDeviceContext()->OMSetBlendState(alpha_blend_state, nullptr, 0xFFFFFFFF);

			for (auto it = m_transparent_models.cbegin(); it != m_transparent_models.cend(); ++it) {
				const ModelNode            * const model_node      = *it;
				const TransformNode        * const model_transform = model_node->GetTransform();
				const Model                * const model           = model_node->GetModel();

				const XMMATRIX object_to_world      = model_transform->GetObjectToWorldMatrix();
				const XMMATRIX object_to_projection = object_to_world * world_to_projection;

				// Cull the model against the view frustum.
				const ViewFrustum view_frustum(object_to_projection);
				const AABB &aabb = model->GetAABB();
				if (!view_frustum.Overlaps(aabb)) {
					continue;
				}

				const XMMATRIX world_to_object      = model_transform->GetWorldToObjectMatrix();
				const XMMATRIX view_to_object       = view_to_world * world_to_object;

				// Update transform constant buffer.
				transform_buffer.SetObjectMatrices(object_to_world, view_to_object);
				m_transform_buffer.UpdateData(transform_buffer);

				// Draw model.
				model->PrepareDrawing();
				model->PrepareShading(m_transform_buffer.Get(), scene_info);
				model->Draw();
			}
		}
	}

	void SceneRenderer::RenderAABBs() {

		// Optimization: Use a separate shader + instancing (1 draw call)

		ID3D11DepthStencilState *depth_stencil_state = RenderingStateCache::Get()->GetDepthDefaultDepthStencilState();
		GetRenderingDeviceContext()->OMSetDepthStencilState(depth_stencil_state, 0);

		for (auto cit = m_cameras.cbegin(); cit != m_cameras.cend(); ++cit) {
			const CameraNode    * const camera_node      = *cit;
			const TransformNode * const camera_transform = camera_node->GetTransform();
			const Camera        * const camera           = camera_node->GetCamera();

			// Sets the viewport of this camera.
			const D3D11_VIEWPORT &viewport = camera_node->GetViewport();
			GetRenderingDeviceContext()->RSSetViewports(1, &viewport);

			const XMMATRIX world_to_view           = camera_transform->GetWorldToViewMatrix();
			const XMMATRIX view_to_world           = camera_transform->GetViewToWorldMatrix();
			const XMMATRIX view_to_projection      = camera->GetViewToProjectionMatrix();
			const XMMATRIX world_to_projection     = world_to_view * view_to_projection;

			// Update scene constant buffer.
			SceneBuffer scene_buffer;
			scene_buffer.m_fog_distance_falloff_start = FLT_MAX;
			scene_buffer.m_fog_distance_falloff_range = FLT_MAX;
			m_scene_buffer.UpdateData(scene_buffer);

			// Create lighting buffer.
			SceneInfo scene_info;
			scene_info.m_scene_buffer       = m_scene_buffer.Get();

			// Create Transform buffer.
			TransformBuffer transform_buffer(world_to_view, view_to_projection);

			ID3D11BlendState *opaque_blend_state = RenderingStateCache::Get()->GetOpaqueBlendState();
			GetRenderingDeviceContext()->OMSetBlendState(opaque_blend_state, nullptr, 0xFFFFFFFF);

			for (auto it = m_opaque_models.cbegin(); it != m_opaque_models.cend(); ++it) {
				const ModelNode            * const model_node      = *it;
				const TransformNode        * const model_transform = model_node->GetTransform();
				const Model                * const model           = model_node->GetModel();

				const XMMATRIX object_to_world      = model_transform->GetObjectToWorldMatrix();
				const XMMATRIX object_to_projection = object_to_world * world_to_projection;

				// Cull the model against the view frustum.
				const ViewFrustum view_frustum(object_to_projection);
				const AABB &aabb = model->GetAABB();
				if (!view_frustum.Overlaps(aabb)) {
					continue;
				}

				Transform box_transform;
				box_transform.SetScale(aabb.Diagonal());
				box_transform.SetTranslation(aabb.Centroid());

				const XMMATRIX world_to_object      = model_transform->GetWorldToObjectMatrix();
				const XMMATRIX box_to_world         = box_transform.GetObjectToParentMatrix() * object_to_world;
				const XMMATRIX world_to_box         = world_to_object * box_transform.GetParentToObjectMatrix();
				const XMMATRIX view_to_box          = view_to_world * world_to_box;

				// Update transform constant buffer.
				transform_buffer.SetObjectMatrices(box_to_world, view_to_box);
				m_transform_buffer.UpdateData(transform_buffer);

				// Draw model.
				const Model * const box_model = m_box->GetModel();
				box_model->PrepareDrawing();
				box_model->PrepareShading(m_transform_buffer.Get(), scene_info);
				box_model->Draw();
			}

			for (auto it = m_transparent_models.cbegin(); it != m_transparent_models.cend(); ++it) {
				const ModelNode            * const model_node      = *it;
				const TransformNode        * const model_transform = model_node->GetTransform();
				const Model                * const model           = model_node->GetModel();

				const XMMATRIX object_to_world      = model_transform->GetObjectToWorldMatrix();
				const XMMATRIX object_to_projection = object_to_world * world_to_projection;

				// Cull the model against the view frustum.
				const ViewFrustum view_frustum(object_to_projection);
				const AABB &aabb = model->GetAABB();
				if (!view_frustum.Overlaps(aabb)) {
					continue;
				}

				Transform box_transform;
				box_transform.SetScale(aabb.Diagonal());
				box_transform.SetTranslation(aabb.Centroid());

				const XMMATRIX world_to_object      = model_transform->GetWorldToObjectMatrix();
				const XMMATRIX box_to_world         = box_transform.GetObjectToParentMatrix() * object_to_world;
				const XMMATRIX world_to_box         = world_to_object * box_transform.GetParentToObjectMatrix();
				const XMMATRIX view_to_box          = view_to_world * world_to_box;

				// Update transform constant buffer.
				transform_buffer.SetObjectMatrices(box_to_world, view_to_box);
				m_transform_buffer.UpdateData(transform_buffer);

				// Draw model.
				const Model * const box_model = m_box->GetModel();
				box_model->PrepareDrawing();
				box_model->PrepareShading(m_transform_buffer.Get(), scene_info);
				box_model->Draw();
			}
		}
	}

	void SceneRenderer::Render2D() {

		ID3D11DepthStencilState *depth_stencil_state = RenderingStateCache::Get()->GetDepthNoneDepthStencilState();
		GetRenderingDeviceContext()->OMSetDepthStencilState(depth_stencil_state, 0);

		ID3D11BlendState *alpha_blend_state = RenderingStateCache::Get()->GetAlphaBlendState();
		GetRenderingDeviceContext()->OMSetBlendState(alpha_blend_state, nullptr, 0xFFFFFFFF);

		m_sprite_batch->Begin();

		for (auto it = m_sprites.cbegin(); it != m_sprites.cend(); ++it) {
			const SpriteNode    * const sprite_node      = *it;
			const Sprite        * const sprite           = sprite_node->GetSprite();

			// Draw sprite.
			sprite->Draw(*m_sprite_batch);
		}

		m_sprite_batch->End();
	}

	void SceneRenderer::Preprocess(const Scene *scene) {
		// Collect active cameras.
		m_cameras.clear();
		m_cameras.reserve(scene->GetNumberOfCameras());
		scene->ForEachCamera([this](const CameraNode *node) {

			if (node->IsPassive()) {
				return;
			}

			m_cameras.push_back(node);

		});

		// Collect active models.
		m_opaque_models.clear();
		m_transparent_models.clear();
		m_opaque_models.reserve(scene->GetNumberOfModels());
		m_transparent_models.reserve(scene->GetNumberOfModels());
		scene->ForEachModel([this](const ModelNode *node) {
			
			if (node->IsPassive() || node->GetModel()->GetNumberOfIndices() == 0) {
				return;
			}

			if (node->GetModel()->GetMaterial()->IsTransparant()) {
				m_transparent_models.push_back(node);
			}
			else {
				m_opaque_models.push_back(node);
			}

		});

		// Collect active ambient light.
		m_ambient_light = RGBSpectrum();
		scene->ForEachAmbientLight([this](const AmbientLightNode *node) {
			if (node->IsActive()) {
				m_ambient_light = node->GetLight()->GetIntensity();
				return;
			}
		});

		// Collect active directional lights.
		m_directional_lights.clear();
		m_directional_lights.reserve(scene->GetNumberOfDirectionalLights());
		scene->ForEachDirectionalLight([this](const DirectionalLightNode *node) {
			
			if (node->IsPassive()) {
				return;
			}

			m_directional_lights.push_back(node);

		});

		// Collect active omni lights.
		m_omni_lights.clear();
		m_omni_lights.reserve(scene->GetNumberOfOmniLights());
		scene->ForEachOmniLight([this](const OmniLightNode *node) {

			if (node->IsPassive()) {
				return;
			}

			m_omni_lights.push_back(node);

		});

		// Collect active spotlights.
		m_spot_lights.clear();
		m_spot_lights.reserve(scene->GetNumberOfSpotLights());
		scene->ForEachSpotLight([this](const SpotLightNode *node) {

			if (node->IsPassive()) {
				return;
			}

			m_spot_lights.push_back(node);

		});

		// Collect active sprites.
		m_sprites.clear();
		m_sprites.reserve(scene->GetNumberOfSprites());
		scene->ForEachSprite([this](const SpriteNode *node) {

			if (node->IsPassive()) {
				return;
			}

			m_sprites.push_back(node);

		});
	}
}