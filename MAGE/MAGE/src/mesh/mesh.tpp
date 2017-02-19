//-----------------------------------------------------------------------------
// Engine Includes
//-----------------------------------------------------------------------------
#pragma region

#include "logging\error.hpp"

#pragma endregion

//-----------------------------------------------------------------------------
// Engine Definitions
//-----------------------------------------------------------------------------
namespace mage {

	template < typename Vertex >
	Mesh< Vertex >::Mesh(const wstring &fname, const Vertex *vertices, size_t nb_vertices, const uint32_t *indices, size_t nb_indices, ComPtr< ID3D11Device2 > device)
		: Resource(fname), m_nb_vertices(nb_vertices), m_nb_indices(nb_indices) {
		
		const HRESULT result_vertex_buffer = SetupVertexBuffer(device, vertices, nb_vertices);
		if (FAILED(result_vertex_buffer)) {
			Error("Vertex buffer initialization failed: %ld.", result_vertex_buffer);
			return;
		}

		const HRESULT result_index_buffer = SetupIndexBuffer(device, indices, nb_indices);
		if (FAILED(result_index_buffer)) {
			Error("Index buffer initialization failed: %ld.", result_index_buffer);
			return;
		}
	}

	template < typename Vertex >
	HRESULT Mesh< Vertex >::SetupVertexBuffer(ComPtr< ID3D11Device2 > device, const Vertex *vertices, size_t nb_vertices) {
		// Describe the buffer resource.
		D3D11_BUFFER_DESC buffer_desc;
		ZeroMemory(&buffer_desc, sizeof(buffer_desc));
		buffer_desc.ByteWidth      = (UINT)(nb_vertices * sizeof(Vertex));
		buffer_desc.Usage          = D3D11_USAGE_DEFAULT;
		buffer_desc.BindFlags      = D3D11_BIND_VERTEX_BUFFER;
		buffer_desc.CPUAccessFlags = 0;

		// Specify data for initializing a subresource.
		D3D11_SUBRESOURCE_DATA init_data;
		ZeroMemory(&init_data, sizeof(init_data));
		init_data.pSysMem          = vertices;

		// Create the vertex buffer.
		// 1. A pointer to a D3D11_BUFFER_DESC structure that describes the buffer.
		// 2. A pointer to a D3D11_SUBRESOURCE_DATA structure that describes the initialization data.
		// 3. Address of a pointer to the ID3D11Buffer interface for the buffer object created.
		const HRESULT result_vertex_buffer = device->CreateBuffer(&buffer_desc, &init_data, m_vertex_buffer.ReleaseAndGetAddressOf());
		if (FAILED(result_vertex_buffer)) {
			Error("CreateBuffer failed: %ld.", result_vertex_buffer);
			return result_vertex_buffer;
		}

		return S_OK;
	}

	template < typename Vertex >
	HRESULT Mesh< Vertex >::SetupIndexBuffer(ComPtr< ID3D11Device2 > device, const uint32_t *indices, size_t nb_indices) {
		// Describe the buffer resource.
		D3D11_BUFFER_DESC buffer_desc;
		ZeroMemory(&buffer_desc, sizeof(buffer_desc));
		buffer_desc.ByteWidth      = (UINT)(nb_indices * sizeof(uint32_t));
		buffer_desc.Usage          = D3D11_USAGE_DEFAULT;
		buffer_desc.BindFlags      = D3D11_BIND_INDEX_BUFFER;
		buffer_desc.CPUAccessFlags = 0;

		// Specify data for initializing a subresource.
		D3D11_SUBRESOURCE_DATA init_data;
		ZeroMemory(&init_data, sizeof(init_data));
		init_data.pSysMem         = indices;

		// Create the index buffer.
		// 1. A pointer to a D3D11_BUFFER_DESC structure that describes the buffer.
		// 2. A pointer to a D3D11_SUBRESOURCE_DATA structure that describes the initialization data.
		// 3. Address of a pointer to the ID3D11Buffer interface for the buffer object created.
		const HRESULT result_index_buffer = device->CreateBuffer(&buffer_desc, &init_data, m_index_buffer.ReleaseAndGetAddressOf());
		if (FAILED(result_index_buffer)) {
			Error("CreateBuffer failed: %ld.", result_index_buffer);
			return result_index_buffer;
		}

		return S_OK;
	}

	template < typename Vertex >
	void Mesh< Vertex >::Update(ComPtr< ID3D11DeviceContext2 > device_context) const {
		// Set the vertex buffer.
		UINT stride = sizeof(Vertex); // The size (in bytes) of the elements that are to be used from a vertex buffer.
		UINT offset = 0;			  // The number of bytes between the first element of a vertex buffer and the first element that will be used.
		// 1. The first input slot for binding.
		// 2. The number of vertex buffers in the array.
		// 3. A pointer to an array of vertex buffers.
		// 4. A pointer to an array of stride values.
		// 5. A pointer to an array of offset values.
		device_context->IASetVertexBuffers(0, 1, m_vertex_buffer.GetAddressOf(), &stride, &offset);

		// Set the index buffer.
		// 1. A pointer to an ID3D11Buffer object.
		// 2. The format of the data in the index buffer.
		// 3. Offset (in bytes) from the start of the index buffer to the first index to use.
		device_context->IASetIndexBuffer(m_index_buffer.Get(), DXGI_FORMAT_R32_UINT, 0);

		// Bind information about the primitive type, and data order that describes input data for the input assembler stage.
		device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}
}