//-----------------------------------------------------------------------------
// Engine Includes
//-----------------------------------------------------------------------------
#pragma region

#include "camera\viewport.hpp"
#include "rendering\renderer.hpp"
#include "math\math_utils.hpp"

#pragma endregion

//-----------------------------------------------------------------------------
// Engine Definitions
//-----------------------------------------------------------------------------
namespace mage {

	const D3D11_VIEWPORT Viewport::GetMaxViewport() noexcept {
		D3D11_VIEWPORT viewport = {};
		viewport.Width    = static_cast< f32 >(Renderer::Get()->GetWidth());
		viewport.Height   = static_cast< f32 >(Renderer::Get()->GetHeight());
		viewport.MaxDepth = 1.0f;
		return viewport;
	}

	f32 Viewport::NormalizeWidth(f32 x) noexcept {
		const f32 width = static_cast< f32 >(Renderer::Get()->GetWidth());
		return Clamp(x * width, 0.0f, width);
	}

	f32 Viewport::NormalizeHeight(f32 x) noexcept {
		const f32 height = static_cast< f32 >(Renderer::Get()->GetHeight());
		return Clamp(x * height, 0.0f, height);
	}
}