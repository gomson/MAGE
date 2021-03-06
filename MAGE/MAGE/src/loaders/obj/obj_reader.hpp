#pragma once

//-----------------------------------------------------------------------------
// Engine Includes
//-----------------------------------------------------------------------------
#pragma region

#include "utils\io\line_reader.hpp"
#include "model\model_output.hpp"
#include "mesh\mesh_descriptor.hpp"

#pragma endregion

//-----------------------------------------------------------------------------
// Engine Declarations and Definitions
//-----------------------------------------------------------------------------
namespace mage {

	/**
	 A class of OBJ file readers for reading meshes.

	 @tparam		VertexT
					The vertex type.
	 */
	template < typename VertexT >
	class OBJReader final : public LineReader {

	public:

		//---------------------------------------------------------------------
		// Constructors and Destructors
		//---------------------------------------------------------------------

		/**
		 Constructs an OBJ reader.

		 @param[in]		model_output
						A reference to a model output for storing the read data 
						from file.
		 @param[in]		mesh_desc
						A reference to a mesh descriptor.
		 */
		explicit OBJReader(ModelOutput< VertexT > &model_output,
			const MeshDescriptor< VertexT > &mesh_desc);
		
		/**
		 Constructs an OBJ reader from the given OBJ reader.

		 @param[in]		reader
						A reference to the OBJ reader to copy.
		 */
		OBJReader(const OBJReader &reader) = delete;

		/**
		 Constructs an OBJ reader by moving the given OBJ reader.

		 @param[in]		reader
						A reference to the OBJ reader to move.
		 */
		OBJReader(OBJReader &&reader);

		/**
		 Destructs this OBJ reader.
		 */
		virtual ~OBJReader();

		//---------------------------------------------------------------------
		// Assignment Operators
		//---------------------------------------------------------------------

		/**
		 Copies the given OBJ reader to this OBJ reader.

		 @param[in]		reader
						A reference to a OBJ reader to copy.
		 @return		A reference to the copy of the given OBJ reader (i.e. 
						this OBJ reader).
		 */
		OBJReader &operator=(const OBJReader &reader) = delete;

		/**
		 Moves the given OBJ reader to this OBJ reader.

		 @param[in]		reader
						A reference to a OBJ reader to move.
		 @return		A reference to the moved OBJ reader (i.e. this OBJ 
						reader).
		 */
		OBJReader &operator=(OBJReader &&reader) = delete;

	private:

		//---------------------------------------------------------------------
		// Member Methods
		//---------------------------------------------------------------------

		/**
		 Pre-process before reading the current file of this OBJ reader.

		 @throws		FormattedException
						Failed to finish the pre-processing successfully.
		 */
		virtual void Preprocess() override;

		/**
		 Reads the given line.

		 @pre			@a line is not equal to @c nullptr.
		 @param[in,out] line
						A pointer to the null-terminated byte string to read.
		 @throws		FormattedException
						Failed to read the given line.
		 */
		virtual void ReadLine(char *line) override;

		/**
		 Post-processes after reading the current file of this OBJ reader.

		 @throws		FormattedException
						Failed to finish post-processing successfully.
		 */
		virtual void Postprocess() override;

		/**
		 Reads a Material Library Include definition.

		 @throws		FormattedException
						Failed to read a Material Library Include definition.
		 */
		void ReadOBJMaterialLibrary();

		/**
		 Reads a Material Usage definition and imports the materials 
		 corresponding to the mesh.

		 @throws		FormattedException
						Failed to read a Material Usage definition.
		 @throws		FormattedException
						Failed to import the materials.
		 */
		void ReadOBJMaterialUse();

		/**
		 Reads a Group definition.

		 @throws		FormattedException
						Failed to read a Group definition.
		 */
		void ReadOBJGroup();

		/**
		 Reads an Object definition.

		 @throws		FormattedException
						Failed to read a Object definition.
		 */
		void ReadOBJObject();

		/**
		 Reads a Smoothing Group definition.

		 @note			A smoothing group is, if present, 
						silently ignored.
		 @throws		FormattedException
						Failed to read a Smoothing Group definition.
		 */
		void ReadOBJSmoothingGroup();

		/**
		 Reads a Vertex Position Coordinates definition.

		 @throws		FormattedException
						Failed to read a Vertex Position Coordinates 
						definition.
		 */
		void ReadOBJVertex();

		/**
		 Reads a Vertex Texture Coordinates definition.

		 @note			Only UV texture coordinates are supported,
						The W component of UVW texture coordinates
						is, if present, silently ignored.
		 @throws		FormattedException
						Failed to read a Vertex Texture Coordinates definition.
		 */
		void ReadOBJVertexTexture();

		/**
		 Reads a Vertex Normal Coordinates definition.

		 @pre			All the vertex normals in the OBJ file are normalized.
		 @throws		FormattedException
						Failed to read a Vertex Normal Coordinates definition.
		 */
		void ReadOBJVertexNormal();

		/**
		 Reads a Face definition.

		 @throws		FormattedException
						Failed to read a Face definition.
		 */
		void ReadOBJFace();

		/**
		 Reads a set of vertex position coordinates.

		 @return		The @c Point3 represented by the next token of this OBJ 
						reader (modified according to the mesh descriptor of 
						this OBj reader).
		 @throws		FormattedException
						Failed to read a @c Point3.
		 */
		const Point3 ReadOBJVertexCoordinates();

		/**
		 Reads a set of vertex normal coordinates.

		 @pre			All the vertex normals in the OBJ file are normalized.
		 @return		The @c Normal3 represented by the next token of this 
						OBJ reader (modified according to the mesh descriptor 
						of this OBj reader).
		 @throws		FormattedException
						Failed to read a @c Normal3.
		 */
		const Normal3 ReadOBJVertexNormalCoordinates();

		/**
		 Reads a set of vertex texture coordinates.

		 @return		The @c UV represented by the next token of this OBJ 
						reader (modified according to the mesh descriptor of 
						this OBj reader).
		 @throws		FormattedException
						Failed to read a @c UV.
		 */
		const UV ReadOBJVertexTextureCoordinates();

		/**
		 Reads a set of face indices.

		 @return		The face indices represented by the next token of this 
						OBJ reader.	A zero indicates the absence of a 
						component.
		 @throws		FormattedException
						Failed to read a Bool variable.
		 */
		const U32x3 ReadOBJVertexIndices();
		
		/**
		 Constructs or retrieves (if already existing) the vertex matching the 
		 given vertex indices.

		 @param[in]		vertex_indices
						A reference to the vertex indices.
		 @return		The vertex matching the given vertex indices 
						@a vertex_indices.
		 */
		const VertexT ConstructVertex(const U32x3 &vertex_indices);

		/**
		 A struct of @c U32x3 comparators for OBJ vertex indices.
		 */
		struct OBJComparatorU32x3 final {

		public:

			/**
			 Compares the two given @c U32x3 vectors against each other.

			 @param[in]		a
							A reference to the first vector.
			 @param[in]		b
							A reference to the second vector.
			 @return		@c true if the @a a is smaller than @a b. @c false 
							otherwise.
			 */
			bool operator()(const U32x3& a, const U32x3& b) const {
				return (a.m_x == b.m_x) ? ((a.m_y == b.m_y) ? 
					(a.m_z < b.m_z) : (a.m_y < b.m_y)) : (a.m_x < b.m_x);
			}
		};

		//---------------------------------------------------------------------
		// Member Variables
		//---------------------------------------------------------------------

		/**
		 A vector containing the read vertex position coordinates of this OBJ 
		 reader.
		 */
		vector< Point3 >  m_vertex_coordinates;

		/**
		 A vector containing the read vertex texture coordinates of this OBJ 
		 reader.
		 */
		vector< UV > m_vertex_texture_coordinates;

		/**
		 A vector containing the read normal texture coordinates of this OBJ 
		 reader.
		 */
		vector< Normal3 > m_vertex_normal_coordinates;

		/**
		 A mapping between vertex position/texture/normal coordinates' indices
		 and the index of a vertex in the vertex buffer (@c m_model_output) of 
		 this OBJ reader.
		 */
		map< U32x3, U32, OBJComparatorU32x3 > m_mapping;
		
		/**
		 A reference to a model output containing the read data of this OBJ 
		 reader.
		 */
		ModelOutput< VertexT > &m_model_output;

		/**
		 A reference to the mesh descriptor for this OBJ reader.
		 */
		const MeshDescriptor< VertexT > &m_mesh_desc;
	};
}

//-----------------------------------------------------------------------------
// Engine Includes
//-----------------------------------------------------------------------------
#pragma region

#include "loaders\obj\obj_reader.tpp"

#pragma endregion