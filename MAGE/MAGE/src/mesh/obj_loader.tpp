#pragma once

//-----------------------------------------------------------------------------
// Engine Includes
//-----------------------------------------------------------------------------
#pragma region

#include "logging\error.hpp"

#pragma endregion

//-----------------------------------------------------------------------------
// Engine Defines
//-----------------------------------------------------------------------------
#pragma region

#define MAGE_OBJ_DELIMITER " \t\n\r"
#define MAGE_OBJ_COMMENT_CHAR '#'
#define MAGE_OBJ_VERTEX_TOKEN "v"
#define MAGE_OBJ_TEXTURE_TOKEN "vt"
#define MAGE_OBJ_NORMAL_TOKEN "vn"
#define MAGE_OBJ_FACE_TOKEN "f"

#pragma endregion

//-----------------------------------------------------------------------------
// Engine Declarations and Definitions
//-----------------------------------------------------------------------------
namespace mage {

	//-------------------------------------------------------------------------
	// Utilities
	//-------------------------------------------------------------------------

	/**
	 A struct of @c XMUINT3 comparators for OBJ vertex indices.
	 */
	struct OBJComparatorXMUINT3 {

	public:

		/**
		 Compares the two given @c XMUINT3 vectors against each other.

		 @param[in]		a
						A reference to the first vector.
		 @param[in]		b
						A reference to the second vector.
		 @return		@c true if the @a a is smaller than @a b.
						@c false otherwise.
		 */
		bool operator()(const XMUINT3& a, const XMUINT3& b) const {
			return (a.x == b.x) ? ((a.y == b.y) ? (a.z < b.z) : (a.y < b.y)) : (a.x < b.x);
		}
	};

	struct OBJBuffer {

	public:

		vector< Point3 >  vertex_coordinates;
		vector< UV >      vertex_texture_coordinates;
		vector< Normal3 > vertex_normal_coordinates;
		map< XMUINT3, uint32_t, OBJComparatorXMUINT3 > mapping;
	};

	XMUINT3 ParseOBJVertexIndices(uint32_t line_number, char **context, char *str = nullptr);
	void ParseOBJVertex(uint32_t line_number, char **context, OBJBuffer &buffer, bool invert_handedness = false);
	void ParseOBJVertexTexture(uint32_t line_number, char **context, OBJBuffer &buffer, bool invert_handedness = false);
	void ParseOBJVertexNormal(uint32_t line_number, char **context, OBJBuffer &buffer, bool invert_handedness = false);

	//-------------------------------------------------------------------------
	// Vertex Specific Utilities
	//-------------------------------------------------------------------------

	template < typename Vertex >
	inline Vertex ConstructVertex(const XMUINT3 &vertex_indices, OBJBuffer &buffer) {
		Vertex vertex;
		if (vertex_indices.x) {
			vertex.p = buffer.vertex_coordinates[vertex_indices.x - 1];
		}
		if (vertex_indices.y) {
			vertex.tex = buffer.vertex_texture_coordinates[vertex_indices.y - 1];
		}
		if (vertex_indices.z) {
			vertex.n = buffer.vertex_normal_coordinates[vertex_indices.z - 1];
		}
		return vertex;
	}

	template <>
	inline VertexPosition ConstructVertex< VertexPosition >(const XMUINT3 &vertex_indices, OBJBuffer &buffer) {
		VertexPosition vertex;
		if (vertex_indices.x) {
			vertex.p = buffer.vertex_coordinates[vertex_indices.x - 1];
		}
		return vertex;
	}

	template <>
	inline VertexPositionNormal ConstructVertex< VertexPositionNormal >(const XMUINT3 &vertex_indices, OBJBuffer &buffer) {
		VertexPositionNormal vertex;
		if (vertex_indices.x) {
			vertex.p = buffer.vertex_coordinates[vertex_indices.x - 1];
		}
		if (vertex_indices.z) {
			vertex.n = buffer.vertex_normal_coordinates[vertex_indices.z - 1];
		}
		return vertex;
	}

	template <>
	inline VertexPositionTexture ConstructVertex< VertexPositionTexture >(const XMUINT3 &vertex_indices, OBJBuffer &buffer) {
		VertexPositionTexture vertex;
		if (vertex_indices.x) {
			vertex.p = buffer.vertex_coordinates[vertex_indices.x - 1];
		}
		if (vertex_indices.y) {
			vertex.tex = buffer.vertex_texture_coordinates[vertex_indices.y - 1];
		}
		return vertex;
	}

	//-------------------------------------------------------------------------
	// Vertex buffer + Index buffer
	//-------------------------------------------------------------------------

	template < typename Vertex >
	static void ParseOBJTriangleFace(uint32_t line_number, char **context, OBJBuffer &buffer,
		vector< Vertex > &vertex_buffer, vector< uint32_t > &index_buffer,
		bool clockwise_order = true) {

		uint32_t indices[3];
		for (size_t i = 0; i < 3; ++i) {
			const XMUINT3 vertex_indices = ParseOBJVertexIndices(line_number, context);
			const map< XMUINT3, uint32_t >::const_iterator it = buffer.mapping.find(vertex_indices);
			if (it != buffer.mapping.cend()) {
				indices[i] = it->second;
			}
			else {
				const uint32_t index = (uint32_t)vertex_buffer.size();
				indices[i] = index;
				vertex_buffer.push_back(ConstructVertex< Vertex >(vertex_indices, buffer));
				buffer.mapping[vertex_indices] = index;
			}
		}

		if (clockwise_order) {
			index_buffer.push_back(indices[2]);
			index_buffer.push_back(indices[1]);
			index_buffer.push_back(indices[0]);
		}
		else {
			index_buffer.push_back(indices[0]);
			index_buffer.push_back(indices[1]);
			index_buffer.push_back(indices[2]);
		}
	}

	template < typename Vertex >
	static void ParseOBJLine(char *line, uint32_t line_number, OBJBuffer &buffer,
		vector< Vertex > &vertex_buffer, vector< uint32_t > &index_buffer,
		bool invert_handedness = false, bool clockwise_order = true) {

		char *context = nullptr;
		const char *token = strtok_s(line, MAGE_OBJ_DELIMITER, &context);

		if (!token || token[0] == MAGE_OBJ_COMMENT_CHAR) {
			return;
		}

		if (str_equals(token, MAGE_OBJ_VERTEX_TOKEN)) {
			ParseOBJVertex(line_number, &context, buffer, invert_handedness);
		}
		else if (str_equals(token, MAGE_OBJ_TEXTURE_TOKEN)) {
			ParseOBJVertexTexture(line_number, &context, buffer, invert_handedness);
		}
		else if (str_equals(token, MAGE_OBJ_NORMAL_TOKEN)) {
			ParseOBJVertexNormal(line_number, &context, buffer, invert_handedness);
		}
		else if (str_equals(token, MAGE_OBJ_FACE_TOKEN)) {
			ParseOBJTriangleFace< Vertex >(line_number, &context, buffer, vertex_buffer, index_buffer, clockwise_order);
		}
		else {
			Warning("Unsupported keyword in OBJ specification at line %u: %s.", line_number, token);
		}

		char *next_token = strtok_s(nullptr, MAGE_OBJ_DELIMITER, &context);
		while (next_token) {
			Warning("Unused token in OBJ specification at line %u: %s.", line_number, next_token);
			next_token = strtok_s(nullptr, MAGE_OBJ_DELIMITER, &context);
		}
	}

	template < typename Vertex >
	HRESULT LoadOBJMeshFromFile(const wstring &fname,
		vector< Vertex > &vertex_buffer, vector< uint32_t > &index_buffer,
		bool invert_handedness, bool clockwise_order) {

		if (!vertex_buffer.empty()) {
			Error("Could not import .obj file: %ls due to non-empty vertex buffer", fname.c_str());
			return E_FAIL;
		}
		if (!index_buffer.empty()) {
			Error("Could not import .obj file: %ls due to non-empty index buffer", fname.c_str());
			return E_FAIL;
		}

		// Open the .obj file.
		FILE *file = nullptr;
		const errno_t result_fopen_s = _wfopen_s(&file, fname.c_str(), L"r");
		if (result_fopen_s) {
			Error("Could not import .obj file: %ls", fname.c_str());
			return E_FAIL;
		}

		// Buffer
		OBJBuffer buffer;

		// Parse the .obj file while populating the buffers.
		char current_line[MAX_PATH];
		uint32_t line_number = 1;
		// Continue reading from the file until the eof is reached.
		while (fgets(current_line, _countof(current_line), file)) {

			ParseOBJLine< Vertex >(current_line, line_number,
				buffer, vertex_buffer, index_buffer,
				invert_handedness, clockwise_order);

			++line_number;
		}

		// Close the script file.
		fclose(file);

		return S_OK;
	}

	template < typename Vertex >
	HRESULT LoadOBJMeshFromMemory(const char *input,
		vector< Vertex > &vertex_buffer, vector< uint32_t > &index_buffer,
		bool invert_handedness, bool clockwise_order) {

		if (!vertex_buffer.empty()) {
			Error("Could not import .obj due to non-empty vertex buffer");
			return E_FAIL;
		}
		if (!index_buffer.empty()) {
			Error("Could not import .obj string due to non-empty index buffer");
			return E_FAIL;
		}

		// Buffer
		OBJBuffer buffer;

		// Parse the .obj string while populating the buffers.
		char current_line[MAX_PATH];
		uint32_t line_number = 1;
		// Continue reading from the string until the eof is reached.
		while (str_gets(current_line, _countof(current_line), &input)) {
			
			ParseOBJLine< Vertex >(current_line, line_number,
				buffer, vertex_buffer, index_buffer,
				invert_handedness, clockwise_order);

			++line_number;
		}

		return S_OK;
	}

	//-------------------------------------------------------------------------
	// Vertex buffer
	//-------------------------------------------------------------------------

	template < typename Vertex >
	static void ParseOBJTriangleFace(uint32_t line_number, char **context,
		OBJBuffer &buffer, vector< Vertex > &vertex_buffer,
		bool clockwise_order = true) {

		Vertex vertices[3];
		for (size_t i = 0; i < 3; ++i) {
			const XMUINT3 vertex_indices = ParseOBJVertexIndices(context);
			vertices[i] = ConstructVertex< Vertex >(vertex_indices, buffer);
		}

		if (clockwise_order) {
			vertex_buffer.push_back(vertices[2]);
			vertex_buffer.push_back(vertices[1]);
			vertex_buffer.push_back(vertices[0]);
		}
		else {
			vertex_buffer.push_back(vertices[0]);
			vertex_buffer.push_back(vertices[1]);
			vertex_buffer.push_back(vertices[2]);
		}
	}

	template < typename Vertex >
	static void ParseOBJLine(char *line, uint32_t line_number,
		OBJBuffer &buffer, vector< Vertex > &vertex_buffer,
		bool invert_handedness, bool clockwise_order) {

		char *context = nullptr;
		const char *token = strtok_s(line, MAGE_OBJ_DELIMITER, &context);

		if (!token || token[0] == MAGE_OBJ_COMMENT_CHAR) {
			return;
		}

		if (str_equals(token, MAGE_OBJ_VERTEX_TOKEN)) {
			ParseOBJVertex(line_number, &context, buffer, invert_handedness);
		}
		else if (str_equals(token, MAGE_OBJ_TEXTURE_TOKEN)) {
			ParseOBJVertexTexture(line_number, &context, buffer, invert_handedness);
		}
		else if (str_equals(token, MAGE_OBJ_NORMAL_TOKEN)) {
			ParseOBJVertexNormal(line_number, &context, buffer, invert_handedness);
		}
		else if (str_equals(token, MAGE_OBJ_FACE_TOKEN)) {
			ParseOBJTriangleFace< Vertex >(line_number, &context, buffer, vertex_buffer, clockwise_order);
		}
		else {
			Warning("Unsupported keyword in OBJ specification at line %u: %s.", line_number, token);
		}

		char *next_token = strtok_s(nullptr, MAGE_OBJ_DELIMITER, &context);
		while (next_token) {
			Warning("Unused token in OBJ specification at line %u: %s.", line_number, next_token);
			next_token = strtok_s(nullptr, MAGE_OBJ_DELIMITER, &context);
		}
	}

	template < typename Vertex >
	HRESULT LoadOBJMeshFromFile(const wstring &fname,
		vector< Vertex > &vertex_buffer,
		bool invert_handedness, bool clockwise_order) {

		if (!vertex_buffer.empty()) {
			Error("Could not import .obj file: %ls due to non-empty vertex buffer", fname.c_str());
			return E_FAIL;
		}

		// Open the .obj file.
		FILE *file = nullptr;
		const errno_t result_fopen_s = _wfopen_s(&file, fname.c_str(), L"r");
		if (result_fopen_s) {
			Error("Could not import .obj file: %ls", fname.c_str());
			return E_FAIL;
		}

		// Buffers
		OBJBuffer buffer;

		// Parse the .obj file while populating the buffers.
		char current_line[MAX_PATH];
		uint32_t line_number = 1;
		// Continue reading from the file until the eof is reached.
		while (fgets(current_line, _countof(current_line), file)) {

			ParseOBJLine< Vertex >(current_line, line_number,
				buffer, vertex_buffer,
				invert_handedness, clockwise_order);

			++line_number;
		}

		// Close the script file.
		fclose(file);

		return S_OK;
	}

	template < typename Vertex >
	HRESULT LoadOBJMeshFromMemory(const char *input,
		vector< Vertex > &vertex_buffer,
		bool invert_handedness, bool clockwise_order) {

		if (!vertex_buffer.empty()) {
			Error("Could not import .obj string due to non-empty vertex buffer");
			return E_FAIL;
		}

		// Buffer
		OBJBuffer buffer;

		// Parse the .obj string while populating the buffers.
		char current_line[MAX_PATH];
		uint32_t line_number = 1;
		// Continue reading from the string until the eof is reached.
		while (str_gets(current_line, _countof(current_line), &input)) {
			
			ParseOBJLine< Vertex >(current_line, line_number,
				buffer, vertex_buffer,
				invert_handedness, clockwise_order);

			++line_number;
		}

		return S_OK;
	}
}