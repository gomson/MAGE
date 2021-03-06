#pragma once

//-----------------------------------------------------------------------------
// Engine Includes
//-----------------------------------------------------------------------------
#pragma region

#include "loaders\obj\obj_reader.hpp"

#pragma endregion

//-----------------------------------------------------------------------------
// Engine Definitions
//-----------------------------------------------------------------------------
namespace mage {

	template < typename VertexT >
	void ImportOBJMeshFromFile(const wstring &fname, 
		ModelOutput< VertexT > &model_output, 
		const MeshDescriptor< VertexT > &mesh_desc) {
		
		OBJReader< VertexT > reader(model_output, mesh_desc);
		reader.ReadFromFile(fname);
	}
}