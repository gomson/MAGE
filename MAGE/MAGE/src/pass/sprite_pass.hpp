#pragma once

//-----------------------------------------------------------------------------
// Engine Includes
//-----------------------------------------------------------------------------
#pragma region

#include "pass\pass_buffer.hpp"
#include "sprite\sprite_batch.hpp"

#pragma endregion

//-----------------------------------------------------------------------------
// Engine Declarations end Definitions
//-----------------------------------------------------------------------------
namespace mage {

	class SpritePass final {

	public:

		//---------------------------------------------------------------------
		// Constructors and Destructors
		//---------------------------------------------------------------------

		SpritePass();
		SpritePass(const SpritePass &render_pass) = delete;
		SpritePass(SpritePass &&render_pass);
		~SpritePass();

		//---------------------------------------------------------------------
		// Assignment Operators
		//---------------------------------------------------------------------

		SpritePass &operator=(const SpritePass &render_pass) = delete;
		SpritePass &operator=(SpritePass &&render_pass) = delete;

		//---------------------------------------------------------------------
		// Member Methods
		//---------------------------------------------------------------------

		void Render(const PassBuffer *scene);

	private:

		//---------------------------------------------------------------------
		// Member Variables
		//---------------------------------------------------------------------

		UniquePtr< SpriteBatch > m_sprite_batch;
	};
}