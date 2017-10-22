//-----------------------------------------------------------------------------
// Engine Includes
//-----------------------------------------------------------------------------
#include "global.hlsli"

//-----------------------------------------------------------------------------
// Constant Buffers
//-----------------------------------------------------------------------------
CBUFFER(Color, SLOT_CBUFFER_COLOR) {
	float4 g_color : packoffset(c0);
};

//-----------------------------------------------------------------------------
// SRVs
//-----------------------------------------------------------------------------
TEXTURE_2D(g_texture, float4, SLOT_SRV_TEXTURE);

//-----------------------------------------------------------------------------
// Pixel Shader
//-----------------------------------------------------------------------------
float4 PS(PSInputPositionNormalTexture input) : SV_Target {
	return GammaToLinear(g_color) 
	       * g_texture.Sample(g_linear_wrap_sampler, input.tex);
}