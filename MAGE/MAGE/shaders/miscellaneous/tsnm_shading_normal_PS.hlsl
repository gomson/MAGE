sampler g_sampler          : register(s0);
Texture2D g_normal_texture : register(t0);

//-----------------------------------------------------------------------------
// Engine Includes
//-----------------------------------------------------------------------------
#include "structures.fx"
#include "normal_mapping.fx"

float4 PS(PSInputPositionNormalTexture input) : SV_Target {
	const float3 n0     = normalize(input.n_view);
	const float3 n_view = TangentSpaceNormalMapping_PerturbNormal(input.p_view, n0, input.tex);
	return float4(InverseBiasX2(n_view), 1.0f);
}