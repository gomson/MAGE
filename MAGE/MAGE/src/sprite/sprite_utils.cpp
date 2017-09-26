//-----------------------------------------------------------------------------
// Engine Includes
//-----------------------------------------------------------------------------
#pragma region

#include "sprite\sprite_utils.hpp"
#include "logging\error.hpp"
#include "logging\exception.hpp"

#pragma endregion

//-----------------------------------------------------------------------------
// Engine Definitions
//-----------------------------------------------------------------------------
namespace mage {

	const XMMATRIX GetViewportTransform(ID3D11DeviceContext2 *device_context, 
		DXGI_MODE_ROTATION rotation_mode) {
		
		Assert(device_context);
		
		u32 nb_of_viewports = 1;
		D3D11_VIEWPORT viewport;
		Pipeline::RS::GetBoundViewports(
			device_context, &nb_of_viewports, &viewport);
		if (nb_of_viewports != 1) {
			throw FormattedException("No viewport is set.");
		}
		
		return GetViewportTransform(viewport, rotation_mode);
	}

	const XMMATRIX GetViewportTransform(ID3D11DeviceContext2 *device_context, 
		DXGI_MODE_ROTATION rotation_mode, D3D11_VIEWPORT *viewport) {
		
		Assert(device_context);
		Assert(viewport);
		
		u32 nb_of_viewports = 1;
		Pipeline::RS::GetBoundViewports(
			device_context, &nb_of_viewports, viewport);
		if (nb_of_viewports != 1) {
			throw FormattedException("No viewport is set.");
		}

		return GetViewportTransform(*viewport, rotation_mode);
	}
	
	const XMMATRIX GetViewportTransform(const D3D11_VIEWPORT &viewport, 
		DXGI_MODE_ROTATION rotation_mode) noexcept {
		
		const f32 scale_x = (viewport.Width  > 0.0f) ? 
							2.0f / viewport.Width  : 0.0f;
		const f32 scale_y = (viewport.Height > 0.0f) ? 
							2.0f / viewport.Height : 0.0f;
		
		switch (rotation_mode) {
		
		case DXGI_MODE_ROTATION_ROTATE90:
			return XMMATRIX
			{
				 0.0f,    -scale_y, 0.0f, 0.0f,
				-scale_x,  0.0f,    0.0f, 0.0f,
				 0.0f,     0.0f,    1.0f, 0.0f,
				 1.0f,     1.0f,    0.0f, 1.0f
			};
		
		case DXGI_MODE_ROTATION_ROTATE180:
			return XMMATRIX
			{
				-scale_x,  0.0f,    0.0f, 0.0f,
				 0.0f,     scale_y, 0.0f, 0.0f,
				 0.0f,     0.0f,    1.0f, 0.0f,
				 1.0f,    -1.0f,    0.0f, 1.0f
			};
		
		case DXGI_MODE_ROTATION_ROTATE270:
			return XMMATRIX
			{
				 0.0f,     scale_y, 0.0f, 0.0f,
				 scale_x,  0.0f,    0.0f, 0.0f,
				 0.0f,     0.0f,    1.0f, 0.0f,
				-1.0f,    -1.0f,    0.0f, 1.0f
			};
		
		default:
			return XMMATRIX
			{
				 scale_x,  0.0f,    0.0f, 0.0f,
				 0.0f,    -scale_y, 0.0f, 0.0f,
				 0.0f,     0.0f,    1.0f, 0.0f,
				-1.0f,     1.0f,    0.0f, 1.0f
			};
		}
	}
}