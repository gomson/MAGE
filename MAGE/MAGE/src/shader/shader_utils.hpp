#pragma once

//-----------------------------------------------------------------------------
// Engine Includes
//-----------------------------------------------------------------------------
#pragma region

#include "utils\type\types.hpp"
#include "rendering\pipeline.hpp"

#pragma endregion

//-----------------------------------------------------------------------------
// Engine Declarations and Definitions
//-----------------------------------------------------------------------------
namespace mage {

	/**
	 Compiles Microsoft High Level Shader Language (HLSL) code into bytecode 
	 for a given shader target.

	 @pre			@a output_blob is not equal to @c nullptr.
	 @param[in]		fname
					A pointer to a constant null-terminated string that 
					contains the name of the file that contains the shader 
					code.
	 @param[in]		entry_point
					A pointer to a constant null-terminated string that 
					contains the name of the shader entry point function where 
					shader execution begins.
	 @param[in]		shader_target
					A pointer to a constant null-terminated string that 
					specifies the shader target or set of shader features to 
					compile against.
	 @param[out]	output_blob
					A pointer to a variable that receives a pointer to the 
					@c ID3DBlob interface that you can use to access the 
					compiled code.
	 */
	HRESULT CompileShaderFromFile(const wstring &fname, 
		const string &entry_point, const string &shader_target, 
		ID3DBlob **output_blob);
}