#pragma once

//-----------------------------------------------------------------------------
// Engine Includes
//-----------------------------------------------------------------------------
#pragma region

#include "platform\windows.hpp"
#include "string\token.hpp"

#pragma endregion

//-----------------------------------------------------------------------------
// Engine Declarations and Definitions
//-----------------------------------------------------------------------------
namespace mage {

	class LineReader {

	public:

		virtual ~LineReader() = default;

		HRESULT ReadFromFile(const wstring &fname, const string &delimiters = MAGE_DEFAULT_DELIMITERS);
		HRESULT ReadFromMemory(const char *input, const string &delimiters = MAGE_DEFAULT_DELIMITERS);

		const wstring &GetFilename() const {
			return m_fname;
		}
		const string &GetDelimiters() const {
			return m_delimiters;
		}

	protected:

		LineReader()
			: m_context(nullptr) {}

		const uint32_t GetCurrentLineNumber() const {
			return m_line_number;
		}

		virtual HRESULT Preprocess();
		virtual HRESULT ReadLine(char *line) = 0;
		virtual HRESULT Postprocess();
		void ReadLineRemaining();

		const char *ReadChars();
		string ReadString();
		string ReadQuotedString();
		bool ReadBool();
		int8_t ReadInt8();
		uint8_t ReadUInt8();
		int16_t ReadInt16();
		uint16_t ReadUInt16();
		int32_t ReadInt32();
		uint32_t ReadUInt32();
		int64_t ReadInt64();
		uint64_t ReadUInt64();
		float ReadFloat();
		double ReadDouble();
		XMFLOAT2 ReadFloat2();
		XMFLOAT3 ReadFloat3();
		XMFLOAT4 ReadFloat4();

		bool HasChars() const;
		bool HasString() const;
		bool HasQuotedString() const;
		bool HasBool() const;
		bool HasInt8() const;
		bool HasUInt8() const;
		bool HasInt16() const;
		bool HasUInt16() const;
		bool HasInt32() const;
		bool HasUInt32() const;
		bool HasInt64() const;
		bool HasUInt64() const;
		bool HasFloat() const;
		bool HasDouble() const;

		char *m_context;

	private:

		LineReader(const LineReader &reader) = delete;
		LineReader &operator=(const LineReader &reader) = delete;

		wstring m_fname;
		string m_delimiters;
		uint32_t m_line_number;
	};
}