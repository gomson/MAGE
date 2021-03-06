#pragma once

//-----------------------------------------------------------------------------
// Engine Includes
//-----------------------------------------------------------------------------
#pragma region

#include "mesh\mesh.hpp"
#include "utils\collection\collection.hpp"

#pragma endregion

//-----------------------------------------------------------------------------
// Engine Declarations and Definitions
//-----------------------------------------------------------------------------
namespace mage {

	/**
	 A class of indexed static meshes.
	 */
	class StaticMesh : public Mesh {

	public:

		//---------------------------------------------------------------------
		// Constructors and Destructors
		//---------------------------------------------------------------------

		/**
		 Constructs a static mesh.

		 @pre			The device associated with the current engine 
						must be loaded.
		 @pre			@a vertices points to an array of at least 
						@a nb_vertices vertices.
		 @pre			@a nb_vertices must be greater than zero.
		 @pre			@a indices points to an array of at least @a nb_indices 
						indices.
		 @pre			@a nb_indices must be greater than zero.
		 @tparam		VertexT
						The vertex type.
		 @tparam		IndexT
						The index type.
		 @param[in]		vertices
						A pointer to an array of vertices.
		 @param[in]		nb_vertices
						The number of vertices.
		 @param[in]		indices
						A pointer to an array of indices.
		 @param[in]		nb_indices
						The number of indices.
		 @param[in]		index_format
						The index format.
		 @param[in]		primitive_topology
						The primitive topology.
		 @throws		FormattedException
						Failed to setup the vertex buffer of the static mesh.
		 @throws		FormattedException
						Failed to setup the index buffer of the static mesh.
		 */
		template < typename VertexT, typename IndexT >
		explicit StaticMesh(
			const VertexT *vertices, size_t nb_vertices,
			const IndexT *indices, size_t nb_indices, 
			DXGI_FORMAT index_format, 
			D3D11_PRIMITIVE_TOPOLOGY primitive_topology 
			= D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		
		/**
		 Constructs a static mesh.

		 @pre			@a device is not equal to @c nullptr.
		 @pre			@a vertices points to an array of at least 
						@a nb_vertices vertices.
		 @pre			@a nb_vertices must be greater than zero.
		 @pre			@a indices points to an array of at least @a nb_indices
						indices.
		 @pre			@a nb_indices must be greater than zero.
		 @tparam		VertexT
						The vertex type.
		 @tparam		IndexT
						The index type.
		 @param[in]		device
						A pointer to the device.
		 @param[in]		vertices
						A pointer to an array of vertices.
		 @param[in]		nb_vertices
						The number of vertices.
		 @param[in]		indices
						A pointer to an array of indices.
		 @param[in]		nb_indices
						The number of indices.
		 @param[in]		index_format
						The index format.
		 @param[in]		primitive_topology
						The primitive topology.
		 @throws		FormattedException
						Failed to setup the vertex buffer of the static mesh.
		 @throws		FormattedException
						Failed to setup the index buffer of the static mesh.
		 */
		template < typename VertexT, typename IndexT >
		explicit StaticMesh(ID3D11Device5 *device, 
			const VertexT *vertices, size_t nb_vertices, 
			const IndexT *indices, size_t nb_indices, 
			DXGI_FORMAT index_format, 
			D3D11_PRIMITIVE_TOPOLOGY primitive_topology 
			= D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		/**
		 Constructs a static mesh.

		 @pre			The device associated with the current engine 
						must be loaded.
		 @pre			The number of vertices must be greater than zero.
		 @pre			The number of indices must be greater than zero.
		 @tparam		VertexT
						The vertex type.
		 @tparam		IndexT
						The index type.
		 @param[in]		vertices
						A reference to a vector of vertices.
		 @param[in]		indices
						A reference to a vector of indices.
		 @param[in]		index_format
						The index format.
		 @param[in]		primitive_topology
						The primitive topology.
		 @throws		FormattedException
						Failed to setup the vertex buffer of the static mesh.
		 @throws		FormattedException
						Failed to setup the index buffer of the static mesh.
		 */
		template < typename VertexT, typename IndexT >
		explicit StaticMesh(
			const vector< VertexT > &vertices, const vector< IndexT > &indices, 
			DXGI_FORMAT index_format, 
			D3D11_PRIMITIVE_TOPOLOGY primitive_topology 
			= D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		/**
		 Constructs a static mesh.

		 @pre			@a device is not equal to @c nullptr.
		 @pre			The number of vertices must be greater than zero.
		 @pre			The number of indices must be greater than zero.
		 @tparam		VertexT
						The vertex type.
		 @tparam		IndexT
						The index type.
		 @param[in]		device
						A pointer to the device.
		 @param[in]		vertices
						A reference to a vector of vertices.
		 @param[in]		indices
						A reference to a vector of indices.
		 @param[in]		index_format
						The index format.
		 @param[in]		primitive_topology
						The primitive topology.
		 @throws		FormattedException
						Failed to setup the vertex buffer of the static mesh.
		 @throws		FormattedException
						Failed to setup the index buffer of the static mesh.
		 */
		template < typename VertexT, typename IndexT >
		explicit StaticMesh(ID3D11Device5 *device,
			const vector< VertexT > &vertices, const vector< IndexT > &indices, 
			DXGI_FORMAT index_format, 
			D3D11_PRIMITIVE_TOPOLOGY primitive_topology 
			= D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		/**
		 Constructs a static mesh from the given static mesh.

		 @param[in]		static_mesh
						A reference to the static mesh to copy.
		 */
		StaticMesh(const StaticMesh &static_mesh) = delete;

		/**
		 Constructs a static mesh by moving the given static mesh.

		 @param[in]		static_mesh
						A reference to the static mesh to move.
		 */
		StaticMesh(StaticMesh &&static_mesh);

		/**
		 Destructs this static mesh.
		 */
		virtual ~StaticMesh();

		//---------------------------------------------------------------------
		// Assignment Operators
		//---------------------------------------------------------------------

		/**
		 Copies the given static mesh to this static mesh.

		 @param[in]		static_mesh
						A reference to the static mesh to copy.
		 @return		A reference to the copy of the given static mesh (i.e. 
						this static mesh).
		 */
		StaticMesh &operator=(const StaticMesh &static_mesh) = delete;

		/**
		 Moves the given static mesh to this static mesh.

		 @param[in]		static_mesh
						A reference to the static mesh to move.
		 @return		A reference to the moved static mesh (i.e. this static 
						mesh).
		 */
		StaticMesh &operator=(StaticMesh &&static_mesh) = delete;

	private:

		//---------------------------------------------------------------------
		// Member Methods
		//---------------------------------------------------------------------

		/**
		 Sets up the vertex buffer of this static mesh.

		 @pre			@a device is not equal to @c nullptr.
		 @pre			@a vertices points to an array of at least 
						@a nb_vertices vertices.
		 @pre			@a nb_vertices must be greater than zero.
		 @tparam		VertexT
						The vertex type.
		 @param[in]		device
						A pointer to the device.
		 @param[in]		vertices
						A pointer to an array of vertices.
		 @param[in]		nb_vertices
						The number of vertices.
		 @throws		FormattedException
						Failed to setup the vertex buffer of this static mesh.
		 */
		template < typename VertexT >
		void SetupVertexBuffer(ID3D11Device5 *device,
			const VertexT *vertices, size_t nb_vertices);

		/**
		 Sets up the index buffer of this static mesh.

		 @pre			@a device is not equal to @c nullptr.
		 @pre			@a indices points to an array of at least @a nb_indices 
						indices.
		 @pre			@a nb_indices must be greater than zero.
		 @tparam		IndexT
						The index type.
		 @param[in]		device
						A pointer to the device.
		 @param[in]		indices
						A pointer to an array of indices.
		 @param[in]		nb_indices
						The number of indices.
		 @throws		FormattedException
						Failed to setup the index buffer of this static mesh.
		 */
		template < typename IndexT >
		void SetupIndexBuffer(ID3D11Device5 *device, 
			const IndexT *indices, size_t nb_indices);
	};
}

//-----------------------------------------------------------------------------
// Engine Includes
//-----------------------------------------------------------------------------
#pragma region

#include "mesh\static_mesh.tpp"

#pragma endregion