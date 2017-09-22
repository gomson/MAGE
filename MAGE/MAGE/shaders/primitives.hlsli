#ifndef MAGE_HEADER_PRIMITIVES
#define MAGE_HEADER_PRIMITIVES

//-----------------------------------------------------------------------------
// Engine Declarations and Definitions
//-----------------------------------------------------------------------------

/**
 Returns the position in NDC space of the vertex corresponding to the given id 
 of a fullscreen triangle.

 @pre			0 <= @a vertex_id < 3
 @param[in]		vertex_id
 @return		The position in NDC space of the vertex corresponding to the 
				given id of a fullscreen triangle.
 */
float4 FullScreenTriangleNDC(uint vertex_id) {
	// v0 = (-1,-1, 0)
	// v1 = (-1, 3, 0)
	// v2 = ( 3,-1, 0)
	const float x = (vertex_id / 2) * 4.0f - 1.0f;
	const float y = (vertex_id % 2) * 4.0f - 1.0f;
	return float4(x, y, 0.0f, 1.0f);
}

/**
 Returns the pair of texture coordinates of the vertex corresponding to the 
 given id of a fullscreen triangle.

 @pre			0 <= @a vertex_id < 3
 @param[in]		vertex_id
 @return		The pair of texture coordinates of the vertex corresponding to 
				the given id of a fullscreen triangle.
 */
float2 FullScreenTriangleUV(uint vertex_id) {
	// v0 = (0,  1)
	// v1 = (0, -1)
	// v2 = (2,  1)
	const float u =        (vertex_id / 2) * 2.0f;
	const float v = 1.0f - (vertex_id % 2) * 2.0f;
	return float2(u, v);
}

#endif // MAGE_HEADER_PRIMITIVES