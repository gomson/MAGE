#pragma once

//-----------------------------------------------------------------------------
// Engine Includes
//-----------------------------------------------------------------------------
#pragma region

#include "memory\memory.hpp"
#include "collection\collection.hpp"
#include "sprite\sprite_utils.hpp"
#include "sprite\sprite_sort_mode.hpp"
#include "sprite\sprite_effects.hpp"
#include "sprite\sprite_transform.hpp"
#include "material\color.hpp"

#pragma endregion

//-----------------------------------------------------------------------------
// System Includes
//-----------------------------------------------------------------------------
#pragma region

#include <functional>

#pragma endregion

//-----------------------------------------------------------------------------
// Engine Declarations and Definitions
//-----------------------------------------------------------------------------
namespace mage {

	struct SpriteInfo {

	};

	class SpriteBatch {

	public:

		SpriteBatch();
		virtual ~SpriteBatch() = default;

		void Begin(SpriteSortMode sort_mode, ID3D11BlendState *blend_state, ID3D11SamplerState *sampler_state,
			ID3D11DepthStencilState *depth_stencil_state, ID3D11RasterizerState *rasterizer_state,
			std::function< void() > SetCustomShadersFunction, XMMATRIX transform = XMMatrixIdentity());
		void Draw(ID3D11ShaderResourceView *texture, const SpriteTransform &transform,
			XMVECTOR color, SpriteEffects effects, float layer_depth);
		void End();

		void SetRotationMode(DXGI_MODE_ROTATION rotation_mode) {
			m_rotation_mode = rotation_mode;
		}
		DXGI_MODE_ROTATION GetRotationMode() const {
			return m_rotation_mode;
		}
		void SetViewport(const D3D11_VIEWPORT &viewport) {
			m_viewport_dirty = true;
			m_viewport = viewport;
		}
		const D3D11_VIEWPORT &GetViewport() const {
			return m_viewport;
		}

	private:

		SpriteBatch(const SpriteBatch &sprite_batch) = delete;
		SpriteBatch(SpriteBatch &&sprite_batch) = delete;
		SpriteBatch &operator=(const SpriteBatch &sprite_batch) = delete;
		SpriteBatch &operator=(SpriteBatch &&sprite_batch) = delete;

		HRESULT InitializeIndexBuffer(ID3D11Device2 &device);
		HRESULT InitializeVertexBuffer(ID3D11Device2 &device);
		HRESULT InitializeShaders(ID3D11Device2 &device);

		void PrepareForRendering(ID3D11DeviceContext &context);

		/**
		 A pointer to the vertex buffer of this sprite batch.
		 */
		ComPtr< ID3D11Buffer > m_vertex_buffer;

		/**
		 A pointer to the index buffer of this sprite batch.
		 */
		ComPtr< ID3D11Buffer > m_index_buffer;


		// Queue of sprites waiting to be drawn.
		UniquePtr< SpriteInfo[] > m_sprite_queue;
		size_t m_sprite_queue_size;
		size_t m_sprite_queue_array_size;
		vector< const SpriteInfo * > m_sorted_sprites;
		vector< ComPtr< ID3D11ShaderResourceView > > m_sprite_texture_references;


		bool m_in_begin_end_pair;

		SpriteSortMode m_sort_mode;;
		ComPtr< ID3D11BlendState > m_blend_state;
		ComPtr< ID3D11SamplerState > m_sampler_state;
		ComPtr< ID3D11DepthStencilState > m_depth_stencil_state;
		ComPtr< ID3D11RasterizerState > m_rasterizer_state;
		std::function< void() > SetCustomShaders;
		XMMATRIX m_transform;

		/**
		 A flag indicating how the back buffers should be rotated 
		 to fit the physical rotation of a monitor.
		 */
		DXGI_MODE_ROTATION m_rotation_mode;

		/**
		 A flag (dirty bit) indicating whether the viewport of
		 this sprite batch has been set.
		 */
		bool m_viewport_dirty;

		/**
		 The viewport of this sprite batch.
		 */
		D3D11_VIEWPORT m_viewport;
	};
}