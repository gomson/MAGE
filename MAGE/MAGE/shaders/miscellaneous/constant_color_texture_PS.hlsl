//-----------------------------------------------------------------------------
// Engine Includes
//-----------------------------------------------------------------------------
#include "hlsl.hpp"
#include "structures.hlsli"

//-----------------------------------------------------------------------------
// Samplers and Textures
//-----------------------------------------------------------------------------
sampler   g_sampler : register(REG_S(SLOT_SAMPLER_DEFAULT));
Texture2D g_texture : register(REG_T(SLOT_SRV_TEXTURE));

//-----------------------------------------------------------------------------
// Pixel Shader
//-----------------------------------------------------------------------------
float4 PS(PSInputPositionNormalTexture input) : SV_Target{
	return g_texture.Sample(g_sampler, input.tex);
}