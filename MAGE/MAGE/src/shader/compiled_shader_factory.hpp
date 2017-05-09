#pragma once

//-----------------------------------------------------------------------------
// Engine Includes
//-----------------------------------------------------------------------------
#pragma region

#include "shader\compiled_shader.hpp"

#pragma endregion

//-----------------------------------------------------------------------------
// Engine Defines
//-----------------------------------------------------------------------------
#pragma region

#define MAGE_GUID_TRANSFORM_VS					L"transform_vs"
#define MAGE_GUID_DIFFUSE_PS					L"diffuse_ps"
#define MAGE_GUID_LAMBERTIAN_PS					L"lambertian_ps"
#define MAGE_GUID_PHONG_PS						L"phong_ps"
#define MAGE_GUID_BLINN_PHONG_PS				L"blinn_phong_ps"
#define MAGE_GUID_MODIFIED_BLINN_PHONG_PS		L"modified_blinn_phong_ps"
#define MAGE_GUID_TSNM_LAMBERTIAN_PS			L"tsnm_lambertian_ps"
#define MAGE_GUID_TSNM_PHONG_PS					L"tsnm_phong_ps"
#define MAGE_GUID_TSNM_BLINN_PHONG_PS			L"tsnm_blinn_phong_ps"
#define MAGE_GUID_TSNM_MODIFIED_BLINN_PHONG_PS	L"tsnm_modified_blinn_phong_ps"
#define MAGE_GUID_SPRITE_VS						L"sprite_vs"
#define MAGE_GUID_SPRITE_PS						L"sprite_ps"

#pragma endregion

//-----------------------------------------------------------------------------
// Engine Declarations and Definitions
//-----------------------------------------------------------------------------
namespace mage {

	//-------------------------------------------------------------------------
	// Factory Methods
	//-------------------------------------------------------------------------

	/**
	 Returns the compiled transform vertex shader.

	 @return		The compiled transform vertex shader.
	 */
	const CompiledVertexShader CreateCompiledTransformVertexShader();

	/**
	 Returns the compiled diffuse pixel shader.

	 @return		The compiled diffuse pixel shader.
	 */
	const CompiledPixelShader CreateCompiledDiffusePixelShader();

	/**
	 Returns the compiled Lambertian pixel shader.

	 @return		The compiled Lambertian pixel shader.
	 */
	const CompiledPixelShader CreateCompiledLambertianPixelShader();

	/**
	 Returns the compiled Phong pixel shader.

	 @return		The compiled Phong pixel shader.
	 */
	const CompiledPixelShader CreateCompiledPhongPixelShader();

	/**
	 Returns the compiled Blinn-Phong pixel shader.

	 @return		The compiled Blinn-Phong pixel shader.
	 */
	const CompiledPixelShader CreateCompiledBlinnPhongPixelShader();

	/**
	 Returns the compiled Modified Blinn-Phong pixel shader.

	 @return		The compiled Modified Blinn-Phong pixel shader.
	 */
	const CompiledPixelShader CreateCompiledModifiedBlinnPhongPixelShader();

	/**
	 Returns the compiled Lambertian bump pixel shader.

	 @return		The compiled Lambertian bump pixel shader.
	 */
	const CompiledPixelShader CreateCompiledLambertianBumpPixelShader();

	/**
	 Returns the compiled Phong bump pixel shader.

	 @return		The compiled Phong bump pixel shader.
	 */
	const CompiledPixelShader CreateCompiledPhongBumpPixelShader();

	/**
	 Returns the compiled Blinn-Phong bump pixel shader.

	 @return		The compiled Blinn-Phong bump pixel shader.
	 */
	const CompiledPixelShader CreateCompiledBlinnPhongBumpPixelShader();

	/**
	 Returns the compiled Modified Blinn-Phong bump pixel shader.

	 @return		The compiled Modified Blinn-Phong bump pixel shader.
	 */
	const CompiledPixelShader CreateCompiledModifiedBlinnPhongBumpPixelShader();

	/**
	 Returns the compiled sprite vertex shader.

	 @return		The compiled sprite vertex shader.
	 */
	const CompiledVertexShader CreateCompiledSpriteVertexShader();

	/**
	 Returns the compiled sprite pixel shader.

	 @return		The compiled sprite pixel shader.
	 */
	const CompiledPixelShader CreateCompiledSpritePixelShader();
}