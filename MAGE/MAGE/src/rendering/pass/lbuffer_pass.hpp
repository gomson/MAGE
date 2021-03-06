#pragma once

//-----------------------------------------------------------------------------
// Engine Includes
//-----------------------------------------------------------------------------
#pragma region

#include "rendering\pass\pass_buffer.hpp"
#include "rendering\buffer\constant_buffer.hpp"
#include "rendering\buffer\structured_buffer.hpp"
#include "rendering\buffer\light_buffer.hpp"
#include "rendering\buffer\shadow_map_buffer.hpp"

#pragma endregion

//-----------------------------------------------------------------------------
// Engine Declarations and Definitions
//-----------------------------------------------------------------------------
namespace mage {

	struct alignas(16) LightCameraInfo final {
		XMMATRIX cview_to_lprojection;
	};

	struct LBufferPass final {

	public:

		//---------------------------------------------------------------------
		// Constructors and Destructors
		//---------------------------------------------------------------------

		LBufferPass();
		LBufferPass(const LBufferPass &buffer) = delete;
		LBufferPass(LBufferPass &&buffer) = default;
		~LBufferPass() = default;

		//---------------------------------------------------------------------
		// Assignment Operators
		//---------------------------------------------------------------------

		LBufferPass &operator=(const LBufferPass &buffer) = delete;
		LBufferPass &operator=(LBufferPass &&buffer) = delete;

		//---------------------------------------------------------------------
		// Member Methods
		//---------------------------------------------------------------------

		size_t GetNumberOfDirectionalLights() const noexcept {
			return m_directional_lights.size();
		}
		size_t GetNumberOfOmniLights() const noexcept {
			return m_omni_lights.size();
		}
		size_t GetNumberOfSpotLights() const noexcept {
			return m_spot_lights.size();
		}
		size_t GetNumberOfDirectionalLightsWithShadowMapping() const noexcept {
			return m_sm_directional_lights.size();
		}
		size_t GetNumberOfOmniLightsWithShadowMapping() const noexcept {
			return m_sm_omni_lights.size();
		}
		size_t GetNumberOfSpotLightsWithShadowMapping() const noexcept {
			return m_sm_spot_lights.size();
		}

		void XM_CALLCONV Render(
			const PassBuffer *scene,
			FXMMATRIX world_to_projection,
			CXMMATRIX world_to_view,
			CXMMATRIX view_to_world);
		
	private:

		//---------------------------------------------------------------------
		// Member Methods
		//---------------------------------------------------------------------

		void UnbindShadowMaps() const noexcept;
		void BindLBuffer() const noexcept;

		void ProcessLightsData(const PassBuffer *scene);

		void XM_CALLCONV ProcessLights(
			const vector< const DirectionalLightNode * > &lights,
			FXMMATRIX world_to_view);
		void XM_CALLCONV ProcessLights(
			const vector< const OmniLightNode * > &lights,
			FXMMATRIX world_to_projection,
			CXMMATRIX world_to_view);
		void XM_CALLCONV ProcessLights(
			const vector< const SpotLightNode * > &lights,
			FXMMATRIX world_to_projection,
			CXMMATRIX world_to_view);

		void XM_CALLCONV ProcessLightsWithShadowMapping(
			const vector< const DirectionalLightNode * > &lights,
			FXMMATRIX world_to_view,
			CXMMATRIX view_to_world);
		void XM_CALLCONV ProcessLightsWithShadowMapping(
			const vector< const OmniLightNode * > &lights,
			FXMMATRIX world_to_projection,
			CXMMATRIX world_to_view,
			CXMMATRIX view_to_world);
		void XM_CALLCONV ProcessLightsWithShadowMapping(
			const vector< const SpotLightNode * > &lights,
			FXMMATRIX world_to_projection,
			CXMMATRIX world_to_view,
			CXMMATRIX view_to_world);

		void SetupShadowMaps();
		void SetupDirectionalShadowMaps();
		void SetupOmniShadowMaps();
		void SetupSpotShadowMaps();

		void RenderShadowMaps(
			const PassBuffer *scene, 
			FXMMATRIX world_to_cview);
		void RenderDirectionalShadowMaps(
			DepthPass *pass, const PassBuffer *scene, 
			FXMMATRIX world_to_cview);
		void RenderOmniShadowMaps(
			DepthPass *pass, const PassBuffer *scene, 
			FXMMATRIX world_to_cview);
		void RenderSpotShadowMaps(
			DepthPass *pass, const PassBuffer *scene, 
			FXMMATRIX world_to_cview);

		//---------------------------------------------------------------------
		// Member Variables
		//---------------------------------------------------------------------

		ID3D11DeviceContext4 * const m_device_context;

		ComPtr< ID3D11SamplerState > m_pcf_sampler;

		ConstantBuffer< LightBuffer > m_light_buffer;
		StructuredBuffer< DirectionalLightBuffer > m_directional_lights;
		StructuredBuffer< OmniLightBuffer > m_omni_lights;
		StructuredBuffer< SpotLightBuffer > m_spot_lights;

		StructuredBuffer< DirectionalLightWithShadowMappingBuffer > 
			m_sm_directional_lights;
		StructuredBuffer< OmniLightWithShadowMappingBuffer > 
			m_sm_omni_lights;
		StructuredBuffer< SpotLightWithShadowMappingBuffer > 
			m_sm_spot_lights;

		UniquePtr< ShadowMapBuffer > m_directional_sms;
		UniquePtr< ShadowCubeMapBuffer > m_omni_sms;
		UniquePtr< ShadowMapBuffer > m_spot_sms;

		vector< LightCameraInfo > m_directional_light_cameras;
		vector< LightCameraInfo > m_omni_light_cameras;
		vector< LightCameraInfo > m_spot_light_cameras;
	};
}