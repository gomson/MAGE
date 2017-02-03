#pragma once

//-----------------------------------------------------------------------------
// Engine Includes
//-----------------------------------------------------------------------------
#pragma region

#include "parallel\atomic.hpp"

#pragma endregion

//-----------------------------------------------------------------------------
// Engine Declarations and Definitions
//-----------------------------------------------------------------------------
namespace mage {

	/**
	 A struct of id generators.
	 */
	struct IdGenerator {

	public:

		/**
		 Constructs an id generator.

		 @param[in]		first_id
						The first id of this id_generator
		 */
		IdGenerator(uint32_t first_id = 0) 
			: m_current_id(first_id) {}

		/**
		 Destructs this id generator.
		 */
		virtual ~IdGenerator() {}

		/**
		 Returns the next id of this id generator.

		 @return		The next id of this id generator.
		 */
		inline uint32_t GetNextId() {
			return (uint32_t)AtomicAdd(&m_current_id, 1);
		}

	private:

		/**
		 Constructs an id generator from the given id generator.

		 @param[in]		id_generator
						The id generator.	
		 */
		IdGenerator(const IdGenerator &id_generator) = delete;

		/**
		 Copies the given id generator to this id generator.

		 @param[in]		id_generator
						The id generator to copy from.
		 @return		A reference to the copy of the given id generator
						(i.e. this id generator).
		 */
		IdGenerator &operator=(const IdGenerator &id_generator) = delete;

		/**
		 The current id of this id generator.
		 */
		AtomicInt32 m_current_id;
	};
}