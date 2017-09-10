//-----------------------------------------------------------------------------
// Engine Includes
//-----------------------------------------------------------------------------
#include "hlsl.hpp"
#include "structures.hlsli"

//-----------------------------------------------------------------------------
// Constant Buffers
//-----------------------------------------------------------------------------
cbuffer PerFrame : register(REG_B(SLOT_CBUFFER_PER_FRAME)) {
	// The object-to-projection transformation matrix.
	float4x4 g_view_to_projection : packoffset(c0);
}

cbuffer PerDraw  : register(REG_B(SLOT_CBUFFER_PER_DRAW)) {
	// The object-to-view transformation matrix.
	float4x4 g_object_to_view     : packoffset(c0);
}

//-----------------------------------------------------------------------------
// Vertex Shader
//-----------------------------------------------------------------------------
float4 VS(VSInputPositionNormalTexture input) : SV_Position {
        // g_object_to_view * g_view_to_projection: eliminates Z-fighting
	return mul(mul(float4(input.p, 1.0f), g_object_to_view), g_view_to_projection);
}