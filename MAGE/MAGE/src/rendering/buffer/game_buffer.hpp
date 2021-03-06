#pragma once

//-----------------------------------------------------------------------------
// Engine Includes
//-----------------------------------------------------------------------------
#pragma region

#include "math\math.hpp"

#pragma endregion

//-----------------------------------------------------------------------------
// Engine Declarations and Definitions
//-----------------------------------------------------------------------------
namespace mage {

	//-------------------------------------------------------------------------
	// GameBuffer
	//-------------------------------------------------------------------------

	/**
	 A struct of game buffers.
	 */
	struct alignas(16) GameBuffer final {

	public:

		//---------------------------------------------------------------------
		// Constructors and Destructors
		//---------------------------------------------------------------------

		/**
		 Constructs a game buffer.
		 */
		GameBuffer()
			: m_display_width(0), 
			m_display_height(0),
			m_display_inv_width_minus1(0.0f),
			m_display_inv_height_minus1(0.0f),
			m_ss_display_width(0),
			m_ss_display_height(0),
			m_ss_display_inv_width_minus1(0.0f),
			m_ss_display_inv_height_minus1(0.0f),
			m_gamma(0.0f), 
			m_inv_gamma(0.0f), 
			m_padding{} {}

		/**
		 Constructs a game buffer from the given game buffer.

		 @param[in]		buffer
						A reference to the game buffer to copy.
		 */
		GameBuffer(const GameBuffer &buffer) = default;
		
		/**
		 Constructs a game buffer by moving the given game buffer.

		 @param[in]		buffer
						A reference to the game buffer to move.
		 */
		GameBuffer(GameBuffer &&buffer) = default;

		/**
		 Destructs this game buffer.
		 */
		~GameBuffer() = default;
		
		//---------------------------------------------------------------------
		// Assignment Operators
		//---------------------------------------------------------------------
		
		/**
		 Copies the given game buffer to this game buffer.

		 @param[in]		buffer
						A reference to the game buffer to copy.
		 @return		A reference to the copy of the given game buffer (i.e. 
						this game buffer).
		 */
		GameBuffer &operator=(const GameBuffer &buffer) = default;

		/**
		 Moves the given game buffer to this game buffer.

		 @param[in]		buffer
						A reference to the game buffer to move.
		 @return		A reference to the moved game buffer (i.e. this game 
						buffer).
		 */
		GameBuffer &operator=(GameBuffer &&buffer) = default;

		//---------------------------------------------------------------------
		// Member Variables: Material
		//---------------------------------------------------------------------

		/**
		 The width of the display of this game buffer.
		 */
		U32 m_display_width;

		/**
		 The height of the display of this game buffer.
		 */
		U32 m_display_height;

		/**
		 The inverse width of the display minus one of this game buffer.
		 */
		F32 m_display_inv_width_minus1;

		/**
		 The inverse height of the display minus one of this game buffer.
		 */
		F32 m_display_inv_height_minus1;

		/**
		 The width of the super-sampled display of this game buffer.
		 */
		U32 m_ss_display_width;

		/**
		 The height of the super-sampled display of this game buffer.
		 */
		U32 m_ss_display_height;

		/**
		 The inverse width of the super-sampled display minus one of this game 
		 buffer.
		 */
		F32 m_ss_display_inv_width_minus1;

		/**
		 The inverse height of the super-sampled display minus one of this game 
		 buffer.
		 */
		F32 m_ss_display_inv_height_minus1;

		/**
		 The gamma exponent of this game buffer.
		 */
		F32 m_gamma;

		/**
		 The inverse gamma exponent of this game buffer.
		 */
		F32 m_inv_gamma;

		/**
		 The padding of this game buffer.
		 */
		U32 m_padding[2];
	};

	static_assert(sizeof(GameBuffer) == 48, "CPU/GPU struct mismatch");
}