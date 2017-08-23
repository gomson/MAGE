#pragma once

//-----------------------------------------------------------------------------
// System Includes
//-----------------------------------------------------------------------------
#pragma region

#include <stdint.h>
#include <random>

#pragma endregion

//-----------------------------------------------------------------------------
// Engine Declarations and Definitions
//-----------------------------------------------------------------------------
namespace mage {

	class RNG final {

	public:

		//-------------------------------------------------------------------------
		// Constructors and Destructors
		//-------------------------------------------------------------------------

		explicit RNG(uint32_t seed = 606418532u)
			: m_generator(),
			m_distribution(std::uniform_real_distribution< float >(0.0f, 1.0f)) {

			Seed(seed);
		}
		
		RNG(const RNG &rng) = default;
		
		RNG(RNG &&rng) = default;
		
		~RNG() = default;

		//-------------------------------------------------------------------------
		// Assignment Operators
		//-------------------------------------------------------------------------

		RNG &operator=(const RNG &v) = delete;
		
		RNG &operator=(RNG &&v) = delete;

		//-------------------------------------------------------------------------
		// Member Methods
		//-------------------------------------------------------------------------

		void Seed(uint32_t seed) noexcept {
			m_generator.seed(seed);
		}

		float UniformFloat() noexcept {
			return m_distribution(m_generator);
		}

		float UniformFloat(float start, float end) noexcept {
			return start + UniformFloat() * (end - start);
		}

	private:

		//-------------------------------------------------------------------------
		// Member Variables
		//-------------------------------------------------------------------------

		std::default_random_engine m_generator;

		std::uniform_real_distribution< float > m_distribution;
	};
}