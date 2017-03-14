#pragma once

//-----------------------------------------------------------------------------
// Engine Includes
//-----------------------------------------------------------------------------
#pragma region

#include "shader\shader.hpp"

#pragma endregion

//-----------------------------------------------------------------------------
// Engine Declarations and Definitions
//-----------------------------------------------------------------------------
namespace mage {

	class EmptyVertexShader : public VertexShader {

	public:

		EmptyVertexShader(const RenderingDevice &device, const wstring &fname, const D3D11_INPUT_ELEMENT_DESC *input_element_desc, uint32_t nb_input_elements)
			: VertexShader(device, fname, input_element_desc, nb_input_elements) {}
		virtual ~EmptyVertexShader() = default;

		virtual void Render(ComPtr< ID3D11DeviceContext2 > device_context, const Material &material, const World &world, const TransformBuffer &transform_buffer) const override;

	private:

		EmptyVertexShader(const EmptyVertexShader &vertex_shader) = delete;
		EmptyVertexShader &operator=(const EmptyVertexShader &vertex_shader) = delete;
	};

	class EmptyPixelShader : public PixelShader {

	public:

		EmptyPixelShader(const RenderingDevice &device, const wstring &fname)
			: PixelShader(device, fname) {}
		virtual ~EmptyPixelShader() = default;

		virtual void Render(ComPtr< ID3D11DeviceContext2 > device_context, const Material &material, const World &world) const override;

	private:

		EmptyPixelShader(const EmptyPixelShader &pixel_shader) = delete;
		EmptyPixelShader &operator=(const EmptyPixelShader &pixel_shader) = delete;
	};
}