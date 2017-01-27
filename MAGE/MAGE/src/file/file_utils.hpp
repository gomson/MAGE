#pragma once

//-----------------------------------------------------------------------------
// Engine Includes
//-----------------------------------------------------------------------------
#pragma region

#include "support.hpp"

#pragma endregion

//-----------------------------------------------------------------------------
// Engine Declarations and Definitions
//-----------------------------------------------------------------------------
namespace mage {

	inline string GetFileExtension(const string &fname) {
		const size_t pos = fname.find_last_of(".");
		return fname.substr(pos + 1);
	}
}