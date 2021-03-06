//-----------------------------------------------------------------------------
// Engine Includes
//-----------------------------------------------------------------------------
#pragma region

#include "utils\logging\error.hpp"
#include "utils\exception\exception.hpp"

#pragma endregion

//-----------------------------------------------------------------------------
// Engine Definitions
//-----------------------------------------------------------------------------
namespace mage {

	FormattedException::FormattedException()
		: exception(), m_text{} {}

	FormattedException::FormattedException(const char *format, ...)
		: FormattedException() {

		Assert(format);

		va_list args;
		
		// Retrieve the additional arguments after format.
		va_start(args, format);

		vsnprintf_s(m_text, _countof(m_text), _TRUNCATE, format, args);
		
		// End using variable argument list.
		va_end(args);

		Error(m_text);
	}

	FormattedException::FormattedException(const char *format, va_list args) 
		: exception(), m_text{} {

		Assert(format);

		vsnprintf_s(m_text, _countof(m_text), _TRUNCATE, format, args);
		
		Error(m_text);
	}

	FormattedException::FormattedException(
		const FormattedException &formatted_exception) = default;

	FormattedException::FormattedException(
		FormattedException &&formatted_exception) = default;

	FormattedException::~FormattedException() = default;

	FormattedException &FormattedException::operator=(
		const FormattedException &formatted_exception) = default;

	FormattedException &FormattedException::operator=(
		FormattedException &&formatted_exception) = default;

	void ThrowIfFailed(bool result) {
		if (!result) {
			throw FormattedException();
		}
	}

	void ThrowIfFailed(bool result, const char *format, ...) {
		if (!result) {
			va_list args;

			// Retrieve the additional arguments after format.
			va_start(args, format);

			const FormattedException exception = FormattedException(format);

			// End using variable argument list.
			va_end(args);

			throw exception;
		}
	}

	void ThrowIfFailed(BOOL result) {
		if (FALSE == result) {
			throw FormattedException();
		}
	}

	void ThrowIfFailed(BOOL result, const char *format, ...) {
		if (FALSE == result) {
			va_list args;

			// Retrieve the additional arguments after format.
			va_start(args, format);

			const FormattedException exception = FormattedException(format);

			// End using variable argument list.
			va_end(args);

			throw exception;
		}
	}

	void ThrowIfFailed(HRESULT result) {
		if (FAILED(result)) {
			throw FormattedException();
		}
	}

	void ThrowIfFailed(HRESULT result, const char *format, ...) {
		if (FAILED(result)) {
			va_list args;

			// Retrieve the additional arguments after format.
			va_start(args, format);

			const FormattedException exception = FormattedException(format);

			// End using variable argument list.
			va_end(args);

			throw exception;
		}
	}
}