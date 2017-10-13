#pragma once

//-----------------------------------------------------------------------------
// Engine Includes
//-----------------------------------------------------------------------------
#pragma region

#include "rendering\pipeline.hpp"

#pragma endregion

//-----------------------------------------------------------------------------
// Engine Declarations and Definitions
//-----------------------------------------------------------------------------
namespace mage {

	/**
	 A struct of rendering state caches.
	 */
	struct RenderingStateCache final {

	public:

		//---------------------------------------------------------------------
		// Class Member Methods
		//---------------------------------------------------------------------

		/**
		 Returns the renderer state cache associated with the current engine.

		 @pre			The renderer associated with the current engine must be 
						loaded.
		 @return		A pointer to the renderer state cache associated with 
						the current engine.
		 @throws		FormattedException
						Failed to setup the rendering states of this rendering 
						state cache.
		 */
		static const RenderingStateCache *Get() noexcept;

		//---------------------------------------------------------------------
		// Constructors and Destructors
		//---------------------------------------------------------------------

		/**
		 Constructs a rendering state cache.

		 @pre			@a device is not equal to @c nullptr.
		 @param[in]		device
						A pointer to the device.
		 @throws		FormattedException
						Failed to setup the rendering states of this rendering 
						state cache.
		 */
		explicit RenderingStateCache(ID3D11Device2 *device);

		/**
		 Constructs a rendering state cache from the given rendering state 
		 cache.

		 @param[in]		rendering_state_cache
						A reference to the rendering state cache to copy.
		 */
		RenderingStateCache(
			const RenderingStateCache &rendering_state_cache) = delete;

		/**
		 Constructs a rendering state cache by moving the given rendering 
		 state cache.

		 @param[in]		rendering_state_cache
						A reference to the rendering state cache to move.
		 */
		RenderingStateCache(
			RenderingStateCache &&rendering_state_cache);

		/**
		 Destructs this rendering state cache.
		 */
		~RenderingStateCache();

		//---------------------------------------------------------------------
		// Assignment Operators
		//---------------------------------------------------------------------	

		/**
		 Copies the given rendering state cache to this rendering state cache.

		 @param[in]		rendering_state_cache
						A reference to the rendering state cache to copy.
		 @return		A reference to the copy of the given rendering state 
						cache (i.e. this rendering state cache).
		 */
		RenderingStateCache &operator=(
			const RenderingStateCache &rendering_state_cache) = delete;

		/**
		 Moves the given rendering state cache to this rendering state cache.

		 @param[in]		rendering_state_cache
						A reference to the rendering state cache to move.
		 @return		A reference to the moved rendering state cache
						(i.e. this rendering state cache).
		 */
		RenderingStateCache &operator=(
			RenderingStateCache &&rendering_state_cache) = delete;

		//---------------------------------------------------------------------
		// Member Methods: Blend States
		//---------------------------------------------------------------------

		/**
		 Binds the opaque blend state of this rendering state cache.

		 @pre			@a device_context is not equal to @c nullptr.
		 @param[in]		device_context
						A pointer to the device context.
		 */
		void BindOpaqueBlendState(
			ID3D11DeviceContext2 *device_context) const noexcept {

			Pipeline::OM::BindBlendState(device_context,
				GetBlendState(BlendStateIndex::Opaque));
		}

		/**
		 Binds the alpha blend state of this rendering state cache.

		 @pre			@a device_context is not equal to @c nullptr.
		 @param[in]		device_context
						A pointer to the device context.
		 */
		void BindAlphaBlendState(
			ID3D11DeviceContext2 *device_context) const noexcept {

			Pipeline::OM::BindBlendState(device_context,
				GetBlendState(BlendStateIndex::Alpha));
		}

		/**
		 Binds the additive blend state of this rendering state cache.

		 @pre			@a device_context is not equal to @c nullptr.
		 @param[in]		device_context
						A pointer to the device context.
		 */
		void BindAdditiveBlendState(
			ID3D11DeviceContext2 *device_context) const noexcept {

			Pipeline::OM::BindBlendState(device_context,
				GetBlendState(BlendStateIndex::Additive));
		}
		
		/**
		 Binds the multiplicative blend state of this rendering state cache.

		 @pre			@a device_context is not equal to @c nullptr.
		 @param[in]		device_context
						A pointer to the device context.
		 */
		void BindMultiplicativeBlendState(
			ID3D11DeviceContext2 *device_context) const noexcept {

			Pipeline::OM::BindBlendState(device_context,
				GetBlendState(BlendStateIndex::Multiplicative));
		}

		/**
		 Binds the bi-multiplicative blend state of this rendering state cache.

		 @pre			@a device_context is not equal to @c nullptr.
		 @param[in]		device_context
						A pointer to the device context.
		 */
		void BindBiMultiplicativeBlendState(
			ID3D11DeviceContext2 *device_context) const noexcept {

			Pipeline::OM::BindBlendState(device_context,
				GetBlendState(BlendStateIndex::BiMultiplicative));
		}

		/**
		 Binds the alpha-to-coverage blend state of this rendering state cache.

		 @pre			@a device_context is not equal to @c nullptr.
		 @param[in]		device_context
						A pointer to the device context.
		 */
		void BindAlphaToCoverageBlendState(
			ID3D11DeviceContext2 *device_context) const noexcept {

			Pipeline::OM::BindBlendState(device_context,
				GetBlendState(BlendStateIndex::AlphaToCoverage));
		}
		
		/**
		 Binds the alpha blend state of this rendering state cache if MSAA is
		 disabled. Otherwise, binds the alpha-to-coverage blend state of this 
		 rendering state cache.

		 @pre			@a device_context is not equal to @c nullptr.
		 @param[in]		device_context
						A pointer to the device context.
		 */
		void BindTransparentBlendState(
			ID3D11DeviceContext2 *device_context) const noexcept;

		//---------------------------------------------------------------------
		// Member Methods: Depth Stencil States
		//---------------------------------------------------------------------
		
		/**
		 Binds the no-depth stencil state of this rendering state cache.

		 @pre			@a device_context is not equal to @c nullptr.
		 @param[in]		device_context
						A pointer to the device context.
		 */
		void BindDepthNoneDepthStencilState(
			ID3D11DeviceContext2 *device_context) const noexcept {

			Pipeline::OM::BindDepthStencilState(device_context,
				GetDepthStencilState(DepthStencilStateIndex::DepthNone));
		}

		/**
		 Binds the less-equal, read-write depth stencil state of this rendering
		 state cache.

		 @pre			@a device_context is not equal to @c nullptr.
		 @param[in]		device_context
						A pointer to the device context.
		 */
		void BindLessEqualDepthReadWriteDepthStencilState(
			ID3D11DeviceContext2 *device_context) const noexcept {

			Pipeline::OM::BindDepthStencilState(device_context,
				GetDepthStencilState(DepthStencilStateIndex::LessEqualDepthReadWrite));
		}

		/**
		 Binds the less-equal, read depth stencil state of this rendering state 
		 cache.

		 @pre			@a device_context is not equal to @c nullptr.
		 @param[in]		device_context
						A pointer to the device context.
		 */
		void BindLessEqualDepthReadDepthStencilState(
			ID3D11DeviceContext2 *device_context) const noexcept {

			Pipeline::OM::BindDepthStencilState(device_context,
				GetDepthStencilState(DepthStencilStateIndex::LessEqualDepthRead));
		}

		/**
		 Binds the less, read-write depth stencil state of this rendering
		 state cache.

		 @pre			@a device_context is not equal to @c nullptr.
		 @param[in]		device_context
						A pointer to the device context.
		 */
		void BindLessDepthReadWriteDepthStencilState(
			ID3D11DeviceContext2 *device_context) const noexcept {

			Pipeline::OM::BindDepthStencilState(device_context,
				GetDepthStencilState(DepthStencilStateIndex::LessDepthReadWrite));
		}

		/**
		 Binds the less, read depth stencil state of this rendering state 
		 cache.

		 @pre			@a device_context is not equal to @c nullptr.
		 @param[in]		device_context
						A pointer to the device context.
		 */
		void BindLessDepthReadDepthStencilState(
			ID3D11DeviceContext2 *device_context) const noexcept {

			Pipeline::OM::BindDepthStencilState(device_context,
				GetDepthStencilState(DepthStencilStateIndex::LessDepthRead));
		}

		//-------------------------------------------------------------------------
		// Member Methods: Rasterizer States
		//-------------------------------------------------------------------------

		/**
		 Binds the no-culling rasterizer state of this rendering state cache.

		 @pre			@a device_context is not equal to @c nullptr.
		 @param[in]		device_context
						A pointer to the device context.
		 */
		void BindCullNoneRasterizerState(
			ID3D11DeviceContext2 *device_context) const noexcept {

			Pipeline::RS::BindState(device_context,
				GetRasterizerState(RasterizerStateIndex::NoCulling));
		}

		/**
		 Binds the clockwise-culling rasterizer state of this rendering state cache.

		 @pre			@a device_context is not equal to @c nullptr.
		 @param[in]		device_context
						A pointer to the device context.
		 */
		void BindCullClockwiseRasterizerState(
			ID3D11DeviceContext2 *device_context) const noexcept {
			
			Pipeline::RS::BindState(device_context,
				GetRasterizerState(RasterizerStateIndex::ClockwiseCulling));
		}

		/**
		 Binds the counter-clockwise-culling rasterizer state of this rendering 
		 state cache.

		 @pre			@a device_context is not equal to @c nullptr.
		 @param[in]		device_context
						A pointer to the device context.
		 */
		void BindCullCounterClockwiseRasterizerState(
			ID3D11DeviceContext2 *device_context) const noexcept {

			Pipeline::RS::BindState(device_context,
				GetRasterizerState(RasterizerStateIndex::CounterClockwiseCulling));
		}

		/**
		 Binds the wireframe rasterizer state of this rendering state cache.

		 @pre			@a device_context is not equal to @c nullptr.
		 @param[in]		device_context
						A pointer to the device context.
		 */
		void BindWireframeRasterizerState(
			ID3D11DeviceContext2 *device_context) const noexcept {

			Pipeline::RS::BindState(device_context,
				GetRasterizerState(RasterizerStateIndex::Wireframe));
		}

		//-------------------------------------------------------------------------
		// Member Methods: Sampler States
		//-------------------------------------------------------------------------

		/**
		 Binds the persistent samplers of this rendering state cache.
		 
		 @pre			@a device_context is not equal to @c nullptr.
		 @param[in]		device_context
						A pointer to the device context.
		 */
		void BindPersistentSamplers(
			ID3D11DeviceContext2 *device_context) const noexcept;

		/**
		 Binds the point sampler state with wrapping of this rendering state cache.

		 @pre			@a device_context is not equal to @c nullptr.
		 @pre			@c SLOT_SAMPLER_VARIABLE_START <= @a slot.
		 @pre			@a slot < @c SLOT_SAMPLER_VARIABLE_END.
		 @tparam		PipelineStageT
						The pipeline stage type.
		 @param[in]		device_context
						A pointer to the device context.
		 @param[in]		slot
						The index into the device's zero-based array to set the 
						sampler to (ranges from @c SLOT_SAMPLER_VARIABLE_START 
						to @c SLOT_SAMPLER_VARIABLE_END).
		 */
		template< typename PipelineStageT >
		void BindPointWrapSamplerState(
			ID3D11DeviceContext2 *device_context, U32 slot) const noexcept;
		
		/**
		 Binds the point sampler state with clamping of this rendering state cache.

		 @pre			@a device_context is not equal to @c nullptr.
		 @pre			@c SLOT_SAMPLER_VARIABLE_START <= @a slot.
		 @pre			@a slot < @c SLOT_SAMPLER_VARIABLE_END.
		 @tparam		PipelineStageT
						The pipeline stage type.
		 @param[in]		device_context
						A pointer to the device context.
		 @param[in]		slot
						The index into the device's zero-based array to set the 
						sampler to (ranges from @c SLOT_SAMPLER_VARIABLE_START 
						to @c SLOT_SAMPLER_VARIABLE_END).
		 */
		template< typename PipelineStageT >
		void BindPointClampSamplerState(
			ID3D11DeviceContext2 *device_context, U32 slot) const noexcept;
		
		/**
		 Binds the point sampler state with mirroring of this rendering state 
		 cache.

		 @pre			@a device_context is not equal to @c nullptr.
		 @pre			@c SLOT_SAMPLER_VARIABLE_START <= @a slot.
		 @pre			@a slot < @c SLOT_SAMPLER_VARIABLE_END.
		 @tparam		PipelineStageT
						The pipeline stage type.
		 @param[in]		device_context
						A pointer to the device context.
		 @param[in]		slot
						The index into the device's zero-based array to set the 
						sampler to (ranges from @c SLOT_SAMPLER_VARIABLE_START 
						to @c SLOT_SAMPLER_VARIABLE_END).
		 */
		template< typename PipelineStageT >
		void BindPointMirrorSamplerState(
			ID3D11DeviceContext2 *device_context, U32 slot) const noexcept;
		
		/**
		 Binds the linear sampler state with wrapping of this rendering state 
		 cache.

		 @pre			@a device_context is not equal to @c nullptr.
		 @pre			@c SLOT_SAMPLER_VARIABLE_START <= @a slot.
		 @pre			@a slot < @c SLOT_SAMPLER_VARIABLE_END.
		 @tparam		PipelineStageT
						The pipeline stage type.
		 @param[in]		device_context
						A pointer to the device context.
		 @param[in]		slot
						The index into the device's zero-based array to set the 
						sampler to (ranges from @c SLOT_SAMPLER_VARIABLE_START 
						to @c SLOT_SAMPLER_VARIABLE_END).
		 */
		template< typename PipelineStageT >
		void BindLinearWrapSamplerState(
			ID3D11DeviceContext2 *device_context, U32 slot) const noexcept;
		
		/**
		 Binds the linear sampler state with clamping of this rendering state 
		 cache.

		 @pre			@a device_context is not equal to @c nullptr.
		 @pre			@c SLOT_SAMPLER_VARIABLE_START <= @a slot.
		 @pre			@a slot < @c SLOT_SAMPLER_VARIABLE_END.
		 @tparam		PipelineStageT
						The pipeline stage type.
		 @param[in]		device_context
						A pointer to the device context.
		 @param[in]		slot
						The index into the device's zero-based array to set the 
						sampler to (ranges from @c SLOT_SAMPLER_VARIABLE_START 
						to @c SLOT_SAMPLER_VARIABLE_END).
		 */
		template< typename PipelineStageT >
		void BindLinearClampSamplerState(
			ID3D11DeviceContext2 *device_context, U32 slot) const noexcept;
		
		/**
		 Binds the linear sampler state with mirroring of this rendering state 
		 cache.

		 @pre			@a device_context is not equal to @c nullptr.
		 @pre			@c SLOT_SAMPLER_VARIABLE_START <= @a slot.
		 @pre			@a slot < @c SLOT_SAMPLER_VARIABLE_END.
		 @tparam		PipelineStageT
						The pipeline stage type.
		 @param[in]		device_context
						A pointer to the device context.
		 @param[in]		slot
						The index into the device's zero-based array to set the 
						sampler to (ranges from @c SLOT_SAMPLER_VARIABLE_START 
						to @c SLOT_SAMPLER_VARIABLE_END).
		 */
		template< typename PipelineStageT >
		void BindLinearMirrorSamplerState(
			ID3D11DeviceContext2 *device_context, U32 slot) const noexcept;

		/**
		 Binds the anisotropic sampler state with wrapping of this rendering 
		 state cache.

		 @pre			@a device_context is not equal to @c nullptr.
		 @pre			@c SLOT_SAMPLER_VARIABLE_START <= @a slot.
		 @pre			@a slot < @c SLOT_SAMPLER_VARIABLE_END.
		 @tparam		PipelineStageT
						The pipeline stage type.
		 @param[in]		device_context
						A pointer to the device context.
		 @param[in]		slot
						The index into the device's zero-based array to set the 
						sampler to (ranges from @c SLOT_SAMPLER_VARIABLE_START 
						to @c SLOT_SAMPLER_VARIABLE_END).
		 */
		template< typename PipelineStageT >
		void BindAnisotropicWrapSamplerState(
			ID3D11DeviceContext2 *device_context, U32 slot) const noexcept;
		
		/**
		 Binds the anisotropic sampler state with clamping of this rendering 
		 state cache.

		 @pre			@a device_context is not equal to @c nullptr.
		 @pre			@c SLOT_SAMPLER_VARIABLE_START <= @a slot.
		 @pre			@a slot < @c SLOT_SAMPLER_VARIABLE_END.
		 @tparam		PipelineStageT
						The pipeline stage type.
		 @param[in]		device_context
						A pointer to the device context.
		 @param[in]		slot
						The index into the device's zero-based array to set the 
						sampler to (ranges from @c SLOT_SAMPLER_VARIABLE_START 
						to @c SLOT_SAMPLER_VARIABLE_END).
		 */
		template< typename PipelineStageT >
		void BindAnisotropicClampSamplerState(
			ID3D11DeviceContext2 *device_context, U32 slot) const noexcept;
		
		/**
		 Binds the anisotropic sampler state with mirroring of this rendering 
		 state cache.

		 @pre			@a device_context is not equal to @c nullptr.
		 @pre			@c SLOT_SAMPLER_VARIABLE_START <= @a slot.
		 @pre			@a slot < @c SLOT_SAMPLER_VARIABLE_END.
		 @tparam		PipelineStageT
						The pipeline stage type.
		 @param[in]		device_context
						A pointer to the device context.
		 @param[in]		slot
						The index into the device's zero-based array to set the 
						sampler to (ranges from @c SLOT_SAMPLER_VARIABLE_START 
						to @c SLOT_SAMPLER_VARIABLE_END).
		 */
		template< typename PipelineStageT >
		void BindAnisotropicMirrorSamplerState(
			ID3D11DeviceContext2 *device_context, U32 slot) const noexcept;

		/**
		 Binds the PCF sampler state of this rendering state cache.

		 @pre			@a device_context is not equal to @c nullptr.
		 @pre			@c SLOT_SAMPLER_VARIABLE_START <= @a slot.
		 @pre			@a slot < @c SLOT_SAMPLER_VARIABLE_END.
		 @tparam		PipelineStageT
						The pipeline stage type.
		 @param[in]		device_context
						A pointer to the device context.
		 @param[in]		slot
						The index into the device's zero-based array to set the 
						sampler to (ranges from @c SLOT_SAMPLER_VARIABLE_START 
						to @c SLOT_SAMPLER_VARIABLE_END).
		 */
		template< typename PipelineStageT >
		void BindPCFSamplerState(
			ID3D11DeviceContext2 *device_context, U32 slot) const noexcept;
		
	private:

		//---------------------------------------------------------------------
		// Member Methods
		//---------------------------------------------------------------------

		/**
		 Setup the rendering states (blend, depth stencil, rasterizer, sampler)
		 of this rendering state cache.

		 @pre			@a device is not equal to @c nullptr.
		 @param[in]		device
						A pointer to the device.
		 @throws		FormattedException
						Failed to setup the rendering states of this rendering 
						state cache.
		 */
		void SetupRenderingStates(ID3D11Device2 *device);

		/**
		 Setup the blend states of this rendering state cache.

		 @pre			@a device is not equal to @c nullptr.
		 @param[in]		device
						A pointer to the device.
		 @throws		FormattedException
						Failed to setup the blend states of this rendering 
						state cache.
		 */
		void SetupBlendStates(ID3D11Device2 *device);
		
		/**
		 Setup the depth stencil states of this rendering state cache.

		 @pre			@a device is not equal to @c nullptr.
		 @param[in]		device
						A pointer to the device.
		 @throws		FormattedException
						Failed to setup the depth stencil states of this 
						rendering state cache.
		 */
		void SetupDepthStencilStates(ID3D11Device2 *device);
		
		/**
		 Setup the rasterizer states of this rendering state cache.

		 @pre			@a device is not equal to @c nullptr.
		 @param[in]		device
						A pointer to the device.
		 @throws		FormattedException
						Failed to setup the rasterizer states of this rendering 
						state cache.
		 */
		void SetupRasterizerStates(ID3D11Device2 *device);
		
		/**
		 Setup the samplers states of this rendering state cache.

		 @pre			@a device is not equal to @c nullptr.
		 @param[in]		device
						A pointer to the device.
		 @throws		FormattedException
						Failed to setup the samplers states of this rendering 
						state cache.
		 */
		void SetupSamplerStates(ID3D11Device2 *device);
		
		/**
		 An enumeration of the different blend state indices for rendering 
		 state caches.

		 This contains:
		 @c Opaque,
		 @c Alpha,
		 @c Additive,
		 @c Multiplicative,
		 @c BiMultiplicative and
		 @c AlphaToCoverage.
		 */
		enum struct BlendStateIndex {
			Opaque           = 0,
			Alpha            = 1,
			Additive         = 2,
			Multiplicative   = 3,
			BiMultiplicative = 4,
			AlphaToCoverage  = 5,
			Count            = 6
		};
		
		/**
		 An enumeration of the different depth stencil state indices for 
		 rendering state caches.

		 This contains:
		 @c DepthNone,
		 @c LessEqualDepthReadWrite,
		 @c LessEqualDepthRead,
		 @c LessDepthReadWrite and
		 @c LessDepthRead.
		 */
		enum struct DepthStencilStateIndex {
			DepthNone               = 0,
			LessEqualDepthReadWrite = 1,
			LessEqualDepthRead      = 2,
			LessDepthReadWrite      = 3,
			LessDepthRead           = 4,
			Count                   = 5
		};
		
		/**
		 An enumeration of the different rasterizer state indices for rendering 
		 state caches.

		 This contains:
		 @c NoCulling,
		 @c ClockwiseCulling,
		 @c CounterClockwiseCulling and
		 @c Wireframe.
		 */
		enum struct RasterizerStateIndex {
			NoCulling               = 0,
			ClockwiseCulling        = 1,
			CounterClockwiseCulling = 2,
			Wireframe               = 3,
			Count                   = 4
		};
		
		/**
		 An enumeration of the different sampler state indices for rendering 
		 state caches.

		 This contains:
		 @c PointWrap,
		 @c PointClamp,
		 @c PointMirror,
		 @c LinearWrap,
		 @c LinearClamp,
		 @c LinearMirror,
		 @c AnisotropicWrap,
		 @c AnisotropicClamp,
		 @c AnisotropicMirror and.
		 @c PCF.
		 */
		enum struct SamplerStateIndex {
			PointWrap         = 0,
			PointClamp        = 1,
			PointMirror       = 2,
			LinearWrap        = 3,
			LinearClamp       = 4,
			LinearMirror      = 5,
			AnisotropicWrap   = 6,
			AnisotropicClamp  = 7,
			AnisotropicMirror = 8,
			PCF               = 9,
			Count             = 10
		};
		
		/**
		 Returns the blend state of this rendering state cache associated to 
		 the given index.

		 @param[in]		index
						The blend state index.
		 @return		A pointer to the blend state of this rendering state 
						cache associated to the given index.
		 */
		ID3D11BlendState *GetBlendState(
			BlendStateIndex index) const noexcept {

			return m_blend_states[static_cast< size_t >(index)].Get();
		}

		/**
		 Returns and releases the address of the blend state of this 
		 rendering state cache associated to the given index.

		 @param[in]		index
						The blend state index.
		 @return		A pointer to a pointer to the blend state of this 
						rendering state cache associated to the given index.
		 */
		ID3D11BlendState **ReleaseAndGetAddressOfBlendState(
			BlendStateIndex index) noexcept {

			return m_blend_states[static_cast< size_t >(index)].ReleaseAndGetAddressOf();
		}
		
		/**
		 Returns the depth stencil state of this rendering state cache 
		 associated to the given index.

		 @param[in]		index
						The depth stencil state index.
		 @return		A pointer to the depth stencil state of this rendering 
						state cache associated to the given index.
		 */
		ID3D11DepthStencilState *GetDepthStencilState(
			DepthStencilStateIndex index) const noexcept {

			return m_depth_stencil_states[static_cast< size_t >(index)].Get();
		}
		
		/**
		 Returns and releases the address of the depth stencil state of this 
		 rendering state cache associated to the given index.

		 @param[in]		index
						The depth stencil state index.
		 @return		A pointer to a pointer to the depth stencil state of 
						this rendering state cache associated to the given index.
		 */
		ID3D11DepthStencilState **ReleaseAndGetAddressOfDepthStencilState(
			DepthStencilStateIndex index) noexcept {

			return m_depth_stencil_states[static_cast< size_t >(index)].ReleaseAndGetAddressOf();
		}
		
		/**
		 Returns the rasterizer state of this rendering state cache associated 
		 to the given index.

		 @param[in]		index
						The rasterizer state index.
		 @return		A pointer to the rasterizer state of this rendering 
						state cache associated to the given index.
		 */
		ID3D11RasterizerState *GetRasterizerState(
			RasterizerStateIndex index) const noexcept {

			return m_rasterizer_states[static_cast< size_t >(index)].Get();
		}
		
		/**
		 Returns and releases the address of the rasterizer state of this 
		 rendering state cache associated to the given index.

		 @param[in]		index
						The rasterizer state index.
		 @return		A pointer to a pointer to the rasterizer state of this 
						rendering state cache associated to the given index.
		 */
		ID3D11RasterizerState **ReleaseAndGetAddressOfRasterizerState(
			RasterizerStateIndex index) noexcept {

			return m_rasterizer_states[static_cast< size_t >(index)].ReleaseAndGetAddressOf();
		}
		
		/**
		 Returns the sampler state of this rendering state cache associated to 
		 the given index.

		 @param[in]		index
						The sampler state index.
		 @return		A pointer to the sampler state of this rendering state 
						cache associated to the given index.
		 */
		ID3D11SamplerState *GetSamplerState(
			SamplerStateIndex index) const noexcept {

			return m_sampler_states[static_cast< size_t >(index)].Get();
		}
		
		/**
		 Returns and releases the address of the sampler state of this 
		 rendering state cache associated to the given index.

		 @param[in]		index
						The sampler state index.
		 @return		A pointer to a pointer to the sampler state of this 
						rendering state cache associated to the given index.
		 */
		ID3D11SamplerState **ReleaseAndGetAddressOfSamplerState(
			SamplerStateIndex index) noexcept {

			return m_sampler_states[static_cast< size_t >(index)].ReleaseAndGetAddressOf();
		}

		//---------------------------------------------------------------------
		// Member Variables
		//---------------------------------------------------------------------

		/**
		 An array containing pointers to the blend states of this rendering 
		 state cache.
		 */
		ComPtr< ID3D11BlendState > m_blend_states[
			static_cast< size_t >(BlendStateIndex::Count)];

		/**
		 An array containing pointers to the depth stencil states of this 
		 rendering state cache.
		 */
		ComPtr< ID3D11DepthStencilState > m_depth_stencil_states[
			static_cast< size_t >(DepthStencilStateIndex::Count)];

		/**
		 An array containing pointers to the rasterizer states of this 
		 rendering state cache.
		 */
		ComPtr< ID3D11RasterizerState > m_rasterizer_states[
			static_cast< size_t >(RasterizerStateIndex::Count)];
		
		/**
		 An array containing pointers to the sampler states of this rendering 
		 state cache.
		 */
		ComPtr< ID3D11SamplerState > m_sampler_states[
			static_cast< size_t >(SamplerStateIndex::Count)];
	};
}

//-----------------------------------------------------------------------------
// Engine Includes
//-----------------------------------------------------------------------------
#pragma region

#include "rendering\rendering_state_cache.tpp"

#pragma endregion
