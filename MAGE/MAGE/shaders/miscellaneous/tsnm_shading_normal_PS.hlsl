//-----------------------------------------------------------------------------
// Engine Configuration
//-----------------------------------------------------------------------------
// Defines			                      | Default
//-----------------------------------------------------------------------------
// MSAA_AS_SSAA                           | not defined

//-----------------------------------------------------------------------------
// Engine Includes
//-----------------------------------------------------------------------------
#include "global.hlsli"
#include "normal_mapping.hlsli"

//-----------------------------------------------------------------------------
// SRVs
//-----------------------------------------------------------------------------
TEXTURE_2D(g_normal_texture, float2, SLOT_SRV_NORMAL);

//-----------------------------------------------------------------------------
// Pixel Shader
//-----------------------------------------------------------------------------
#ifdef MSAA_AS_SSAA
float4 PS(PSInputPositionNormalTexture input, uint index : SV_SampleIndex) : SV_Target {
#else  // MSAA_AS_SSAA
float4 PS(PSInputPositionNormalTexture input) : SV_Target {
#endif // MSAA_AS_SSAA

	// Obtain the tangent-space normal coefficients in the [-1,1] range.
	const float3 c = UnpackNormal(
		g_normal_texture.Sample(g_linear_wrap_sampler, input.tex2));
	// Perturb the view-space normal.
	const float3 n_view 
		= PerturbNormal(input.p_view, normalize(input.n_view), input.tex2, c);
	
	// Converts the [-1,1] range to the [0,1] range.
	return float4(SNORMtoUNORM(n_view), 1.0f);
}