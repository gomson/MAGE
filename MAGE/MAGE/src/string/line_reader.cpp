//-----------------------------------------------------------------------------
// Engine Includes
//-----------------------------------------------------------------------------
#pragma region

#include "string\line_reader.hpp"
#include "string\string_utils.hpp"
#include "logging\error.hpp"
#include "logging\exception.hpp"

#pragma endregion

//-----------------------------------------------------------------------------
// Engine Definitions
//-----------------------------------------------------------------------------
namespace mage {

	LineReader::LineReader()
		: m_context(nullptr), m_file_stream(nullptr), m_fname(),
		m_delimiters(g_default_delimiters), m_line_number(0) {}

	LineReader::LineReader(LineReader &&reader) = default;

	LineReader::~LineReader() = default;

	void LineReader::ReadFromFile(const wstring &fname, const string &delimiters) {
		m_fname = fname;
		m_delimiters = delimiters;
		
		// Open the file.
		FILE *file;
		const errno_t result_fopen_s = _wfopen_s(&file, GetFilename().c_str(), L"r");
		if (result_fopen_s) {
			throw FormattedException("%ls: could not open file.", GetFilename().c_str());
		}
		m_file_stream.reset(file);

		Preprocess();

		char current_line[MAX_PATH];
		m_line_number = 1;
		// Continue reading from the file until the eof is reached.
		while (fgets(current_line, _countof(current_line), m_file_stream.get())) {
			ReadLine(current_line);
			++m_line_number;
		}

		Postprocess();

		m_context = nullptr;
	}

	void LineReader::ReadFromMemory(const char *input, const string &delimiters) {
		Assert(input);
		
		m_fname = L"input string";
		m_delimiters = delimiters;
		m_file_stream.reset(nullptr);

		Preprocess();

		char current_line[MAX_PATH];
		m_line_number = 1;
		// Continue reading from the file until the eof is reached.
		while (str_gets(current_line, _countof(current_line), &input)) {
			ReadLine(current_line);
			++m_line_number;
		}

		Postprocess();

		m_context = nullptr;
	}

	void LineReader::ReadLineRemaining() {
		char *next_token = strtok_s(nullptr, GetDelimiters().c_str(), &m_context);
		while (next_token) {
			Warning("%ls: line %u: unused token: %s.", 
				GetFilename().c_str(), GetCurrentLineNumber(), next_token);
			next_token = strtok_s(nullptr, GetDelimiters().c_str(), &m_context);
		}
	}
	
	const char *LineReader::ReadChars() {
		char *result;
		const TokenResult token_result 
			= mage::ReadChars(nullptr, &m_context, &result, GetDelimiters().c_str());

		switch (token_result) {
		case TokenResult::Valid: {
			return result;
		}
		default: {
			throw FormattedException(
				"%ls: line %u: no char string value found.",
				GetFilename().c_str(), GetCurrentLineNumber());
		}
		}
	}
	
	const string LineReader::ReadString() {
		string result;
		const TokenResult token_result 
			= mage::ReadString(nullptr, &m_context, result, GetDelimiters().c_str());

		switch (token_result) {
		case TokenResult::Valid: {
			return result;
		}
		default: {
			throw FormattedException(
				"%ls: line %u: no string value found.", 
				GetFilename().c_str(), GetCurrentLineNumber());
		}
		}
	}
	
	const string LineReader::ReadQuotedString() {
		string result;
		const TokenResult token_result 
			= mage::ReadQuotedString(nullptr, &m_context, result, GetDelimiters().c_str());

		switch (token_result) {
		case TokenResult::Valid: {
			return result;
		}
		case TokenResult::None: {
			throw FormattedException(
				"%ls: line %u: no quoted string value found.", 
				GetFilename().c_str(), GetCurrentLineNumber());
		}
		default: {
			throw FormattedException(
				"%ls: line %u: invalid quoted string value found.", 
				GetFilename().c_str(), GetCurrentLineNumber());
		}
		}
	}
	
	bool LineReader::ReadBool() {
		bool result;
		const TokenResult token_result 
			= mage::ReadBool(nullptr, &m_context, result, GetDelimiters().c_str());

		switch (token_result) {
		case TokenResult::Valid: {
			return result;
		}
		case TokenResult::None: {
			throw FormattedException(
				"%ls: line %u: no bool value found.", 
				GetFilename().c_str(), GetCurrentLineNumber());
		}
		default: {
			throw FormattedException(
				"%ls: line %u: invalid bool value found.", 
				GetFilename().c_str(), GetCurrentLineNumber());
		}
		}
	}
	
	i8 LineReader::ReadI8() {
		i8 result;
		const TokenResult token_result 
			= mage::ReadI8(nullptr, &m_context, result, GetDelimiters().c_str());

		switch (token_result) {
		case TokenResult::Valid: {
			return result;
		}
		case TokenResult::None: {
			throw FormattedException(
				"%ls: line %u: no 8 bit integer value found.", 
				GetFilename().c_str(), GetCurrentLineNumber());
		}
		default: {
			throw FormattedException(
				"%ls: line %u: invalid 8 bit integer value found.", 
				GetFilename().c_str(), GetCurrentLineNumber());
		}
		}
	}
	
	u8 LineReader::ReadU8() {
		u8 result;
		const TokenResult token_result 
			= mage::ReadU8(nullptr, &m_context, result, GetDelimiters().c_str());

		switch (token_result) {
		case TokenResult::Valid: {
			return result;
		}
		case TokenResult::None: {
			throw FormattedException(
				"%ls: line %u: no 8 bit unsigned integer value found.", 
				GetFilename().c_str(), GetCurrentLineNumber());
		}
		default: {
			throw FormattedException(
				"%ls: line %u: invalid 8 bit unsigned integer value found.", 
				GetFilename().c_str(), GetCurrentLineNumber());
		}
		}
	}
	
	i16 LineReader::ReadI16() {
		i16 result;
		const TokenResult token_result 
			= mage::ReadI16(nullptr, &m_context, result, GetDelimiters().c_str());

		switch (token_result) {
		case TokenResult::Valid: {
			return result;
		}
		case TokenResult::None: {
			throw FormattedException(
				"%ls: line %u: no 16 bit integer value found.", 
				GetFilename().c_str(), GetCurrentLineNumber());
		}
		default: {
			throw FormattedException(
				"%ls: line %u: invalid 16 bit integer value found.", 
				GetFilename().c_str(), GetCurrentLineNumber());
		}
		}
	}
	
	u16 LineReader::ReadU16() {
		u16 result;
		const TokenResult token_result 
			= mage::ReadU16(nullptr, &m_context, result, GetDelimiters().c_str());

		switch (token_result) {
		case TokenResult::Valid: {
			return result;
		}
		case TokenResult::None: {
			throw FormattedException(
				"%ls: line %u: no 16 bit unsigned integer value found.", 
				GetFilename().c_str(), GetCurrentLineNumber());
		}
		default: {
			throw FormattedException(
				"%ls: line %u: invalid 16 bit unsigned integer value found.", 
				GetFilename().c_str(), GetCurrentLineNumber());
		}
		}
	}
	
	i32 LineReader::ReadI32() {
		i32 result;
		const TokenResult token_result 
			= mage::ReadI32(nullptr, &m_context, result, GetDelimiters().c_str());

		switch (token_result) {
		case TokenResult::Valid: {
			return result;
		}
		case TokenResult::None: {
			throw FormattedException(
				"%ls: line %u: no 32 bit integer value found.", 
				GetFilename().c_str(), GetCurrentLineNumber());
		}
		default: {
			throw FormattedException(
				"%ls: line %u: invalid 32 bit integer value found.", 
				GetFilename().c_str(), GetCurrentLineNumber());
		}
		}
	}
	
	u32 LineReader::ReadU32() {
		u32 result;
		const TokenResult token_result 
			= mage::ReadU32(nullptr, &m_context, result, GetDelimiters().c_str());

		switch (token_result) {
		case TokenResult::Valid: {
			return result;
		}
		case TokenResult::None: {
			throw FormattedException(
				"%ls: line %u: no unsigned 32 bit integer value found.", 
				GetFilename().c_str(), GetCurrentLineNumber());
		}
		default: {
			throw FormattedException(
				"%ls: line %u: invalid unsigned 32 bit integer value found.", 
				GetFilename().c_str(), GetCurrentLineNumber());
		}
		}
	}
	
	i64 LineReader::ReadI64() {
		i64 result;
		const TokenResult token_result 
			= mage::ReadI64(nullptr, &m_context, result, GetDelimiters().c_str());

		switch (token_result) {
		case TokenResult::Valid: {
			return result;
		}
		case TokenResult::None: {
			throw FormattedException(
				"%ls: line %u: no 64 bit integer value found.", 
				GetFilename().c_str(), GetCurrentLineNumber());
		}
		default: {
			throw FormattedException(
				"%ls: line %u: invalid 64 bit integer value found.", 
				GetFilename().c_str(), GetCurrentLineNumber());
		}
		}
	}
	
	u64 LineReader::ReadU64() {
		u64 result;
		const TokenResult token_result 
			= mage::ReadU64(nullptr, &m_context, result, GetDelimiters().c_str());

		switch (token_result) {
		case TokenResult::Valid: {
			return result;
		}
		case TokenResult::None: {
			throw FormattedException(
				"%ls: line %u: no 64 bit unsigned integer value found.", 
				GetFilename().c_str(), GetCurrentLineNumber());
		}
		default: {
			throw FormattedException(
				"%ls: line %u: invalid 64 bit unsigned integer value found.", 
				GetFilename().c_str(), GetCurrentLineNumber());
		}
		}
	}
	
	f32 LineReader::ReadF32() {
		f32 result;
		const TokenResult token_result 
			= mage::ReadFloat(nullptr, &m_context, result, GetDelimiters().c_str());

		switch (token_result) {
		case TokenResult::Valid: {
			return result;
		}
		case TokenResult::None: {
			throw FormattedException(
				"%ls: line %u: no f32 value found.", GetFilename().c_str(), GetCurrentLineNumber());
		}
		default: {
			throw FormattedException(
				"%ls: line %u: invalid f32 value found.", GetFilename().c_str(), GetCurrentLineNumber());
		}
		}
	}
	
	f64 LineReader::ReadF64() {
		f64 result;
		const TokenResult token_result 
			= mage::ReadDouble(nullptr, &m_context, result, GetDelimiters().c_str());

		switch (token_result) {
		case TokenResult::Valid: {
			return result;
		}
		case TokenResult::None: {
			throw FormattedException(
				"%ls: line %u: no f64 value found.", 
				GetFilename().c_str(), GetCurrentLineNumber());
		}
		default: {
			throw FormattedException(
				"%ls: line %u: invalid f64 value found.", 
				GetFilename().c_str(), GetCurrentLineNumber());
		}
		}
	}
	
	const XMFLOAT2 LineReader::ReadFloat2() {
		XMFLOAT2 result;
		const TokenResult token_result 
			= mage::ReadFloat2(nullptr, &m_context, result, GetDelimiters().c_str());

		switch (token_result) {
		case TokenResult::Valid: {
			return result;
		}
		case TokenResult::None: {
			throw FormattedException(
				"%ls: line %u: no f322 value found.", 
				GetFilename().c_str(), GetCurrentLineNumber());
		}
		default: {
			throw FormattedException(
				"%ls: line %u: invalid f322 value found.", 
				GetFilename().c_str(), GetCurrentLineNumber());
		}
		}
	}
	
	const XMFLOAT3 LineReader::ReadFloat3() {
		XMFLOAT3 result;
		const TokenResult token_result 
			= mage::ReadFloat3(nullptr, &m_context, result, GetDelimiters().c_str());

		switch (token_result) {
		case TokenResult::Valid: {
			return result;
		}
		case TokenResult::None: {
			throw FormattedException(
				"%ls: line %u: no f323 value found.", 
				GetFilename().c_str(), GetCurrentLineNumber());
		}
		default: {
			throw FormattedException(
				"%ls: line %u: invalid f323 value found.", 
				GetFilename().c_str(), GetCurrentLineNumber());
		}
		}
	}
	
	const XMFLOAT4 LineReader::ReadFloat4() {
		XMFLOAT4 result;
		const TokenResult token_result 
			= mage::ReadFloat4(nullptr, &m_context, result, GetDelimiters().c_str());

		switch (token_result) {
		case TokenResult::Valid: {
			return result;
		}
		case TokenResult::None: {
			throw FormattedException(
				"%ls: line %u: no f324 value found.", 
				GetFilename().c_str(), GetCurrentLineNumber());
		}
		default: {
			throw FormattedException(
				"%ls: line %u: invalid f324 value found.", 
				GetFilename().c_str(), GetCurrentLineNumber());
		}
		}
	}

	bool LineReader::HasChars() const {
		return mage::HasChars(m_context, GetDelimiters().c_str()) 
			== TokenResult::Valid;
	}
	
	bool LineReader::HasString() const {
		return mage::HasString(m_context, GetDelimiters().c_str()) 
			== TokenResult::Valid;
	}
	
	bool LineReader::HasQuotedString() const {
		return mage::HasQuotedString(m_context, GetDelimiters().c_str()) 
			== TokenResult::Valid;
	}
	
	bool LineReader::HasBool() const {
		return mage::HasBool(m_context, GetDelimiters().c_str()) 
			== TokenResult::Valid;
	}
	
	bool LineReader::HasI8() const {
		return mage::HasI8(m_context, GetDelimiters().c_str()) 
			== TokenResult::Valid;
	}
	
	bool LineReader::HasU8() const {
		return mage::HasU8(m_context, GetDelimiters().c_str()) 
			== TokenResult::Valid;
	}
	
	bool LineReader::HasI16() const {
		return mage::HasI16(m_context, GetDelimiters().c_str()) 
			== TokenResult::Valid;
	}
	
	bool LineReader::HasU16() const {
		return mage::HasU16(m_context, GetDelimiters().c_str()) 
			== TokenResult::Valid;
	}
	
	bool LineReader::HasI32() const {
		return mage::HasI32(m_context, GetDelimiters().c_str()) 
			== TokenResult::Valid;
	}
	
	bool LineReader::HasU32() const {
		return mage::HasU32(m_context, GetDelimiters().c_str()) 
			== TokenResult::Valid;
	}
	
	bool LineReader::HasI64() const {
		return mage::HasI64(m_context, GetDelimiters().c_str()) 
			== TokenResult::Valid;
	}
	
	bool LineReader::HasU64() const {
		return mage::HasU64(m_context, GetDelimiters().c_str()) 
			== TokenResult::Valid;
	}
	
	bool LineReader::HasF32() const {
		return mage::HasF32(m_context, GetDelimiters().c_str()) 
			== TokenResult::Valid;
	}
	
	bool LineReader::HasF64() const {
		return mage::HasF64(m_context, GetDelimiters().c_str()) 
			== TokenResult::Valid;
	}
}