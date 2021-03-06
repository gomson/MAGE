//-----------------------------------------------------------------------------
// Engine Includes
//-----------------------------------------------------------------------------
#include "global.hlsli"
#include "primitive\fullscreen_triangle.hlsli"

//-----------------------------------------------------------------------------
// Vertex Shader
//-----------------------------------------------------------------------------

// Number of vertices: 3
// Topology: D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST

PSInputNDCPosition VS(uint vertex_id : SV_VertexID) {
	PSInputNDCPosition output;
	output.p     = NearFullScreenTriangleNDC(vertex_id);
	output.p_ndc = output.p.xyz;
	return output;
}