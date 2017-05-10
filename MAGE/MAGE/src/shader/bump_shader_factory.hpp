#pragma once

//-----------------------------------------------------------------------------
// Engine Includes
//-----------------------------------------------------------------------------
#pragma region

#include "shader\bump_shader.hpp"

#pragma endregion

//-----------------------------------------------------------------------------
// Engine Declarations
//-----------------------------------------------------------------------------
namespace mage {

	//-------------------------------------------------------------------------
	// Factory Methods
	//-------------------------------------------------------------------------

	/**
	 Creates a Lambertian bump shader.

	 @pre			The current engine must be loaded.
	 @return		The Lambertian bump shader.
	 @throws		FormattedException
					Failed to create the shader.
	 */
	const CombinedShader CreateLambertianBumpShader();

	/**
	 Creates a Phong bump shader.

	 @pre			The current engine must be loaded.
	 @return		The Phong bump shader.
	 @throws		FormattedException
					Failed to create the shader.
	 */
	const CombinedShader CreatePhongBumpShader();

	/**
	 Creates a Modified Phong bump shader.

	 @pre			The current engine must be loaded.
	 @return		The Modified Phong bump shader.
	 @throws		FormattedException
					Failed to create the shader.
	 */
	const CombinedShader CreateModifiedPhongBumpShader();

	/**
	 Creates a Blinn-Phong bump shader.

	 @pre			The current engine must be loaded.
	 @return		The Blinn-Phong bump shader.
	 @throws		FormattedException
					Failed to create the shader.
	 */
	const CombinedShader CreateBlinnPhongBumpShader();

	/**
	 Creates a Modified Blinn-Phong bump shader.

	 @pre			The current engine must be loaded.
	 @return		The Modified Blinn-Phong bump shader.
	 @throws		FormattedException
					Failed to create the shader.
	 */
	const CombinedShader CreateModifiedBlinnPhongBumpShader();

	/**
	 Creates a Ward bump shader.

	 @pre			The current engine must be loaded.
	 @return		The Ward bump shader.
	 @throws		FormattedException
					Failed to create the shader.
	 */
	const CombinedShader CreateWardBumpShader();

	/**
	 Creates a Ward-Duer bump shader.

	 @pre			The current engine must be loaded.
	 @return		The Ward-Duer bump shader.
	 @throws		FormattedException
					Failed to create the shader.
	 */
	const CombinedShader CreateWardDuerBumpShader();

	/**
	 Creates a Cook-Torrance bump shader.

	 @pre			The current engine must be loaded.
	 @return		The Cook-Torrance bump shader.
	 @throws		FormattedException
					Failed to create the shader.
	 */
	const CombinedShader CreateCookTorranceBumpShader();
}