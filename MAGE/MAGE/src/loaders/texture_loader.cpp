//-----------------------------------------------------------------------------
// Engine Includes
//-----------------------------------------------------------------------------
#pragma region

#include "loaders\texture_loader.hpp"
#include "loaders\dds\dds_loader.hpp"
#include "loaders\dds\screen_grab.hpp"
#include "loaders\wic\wic_loader.hpp"
#include "utils\file\file_utils.hpp"
#include "utils\logging\error.hpp"
#include "utils\exception\exception.hpp"

#pragma endregion

//-----------------------------------------------------------------------------
// System Includes
//-----------------------------------------------------------------------------
#pragma region

#include <wincodec.h>

#pragma endregion

//-----------------------------------------------------------------------------
// Engine Definitions
//-----------------------------------------------------------------------------
namespace mage {

	void ImportTextureFromFile(const wstring &fname, 
		ID3D11Device5 *device, 
		ID3D11ShaderResourceView **texture_srv) {
		
		Assert(device);
		Assert(texture_srv);
		
		const wstring extension = GetFileExtension(fname);

		if (extension == L"dds" || extension == L"DDS") {
			const HRESULT result = DirectX::CreateDDSTextureFromFile(
				device, fname.c_str(), nullptr, texture_srv);
			ThrowIfFailed(result, "Texture importing failed: %08X.", result);
		}
		else {
			const HRESULT result = DirectX::CreateWICTextureFromFile(
				device, fname.c_str(), nullptr, texture_srv);
			ThrowIfFailed(result, "Texture importing failed: %08X.", result);
		}
	}

	/**
	 Returns the WIC container format associated with the given image file 
	 extension.

	 @param[in]		extension
					A reference to the file extension.
	 @return		@c GUID_NULL if no WIC container format is associated with 
					the given file extension.
	 @return		The WIC container format associated with the given image 
					file extension.
	 */
	inline const GUID GetGUIDContainerFormat(const wstring &extension) noexcept {
		if (extension == L"png" || extension == L"PNG") {
			return GUID_ContainerFormatPng;
		}
		else if (extension == L"jpe"  || extension == L"JPE"
			  || extension == L"jpeg" || extension == L"JPEG"
			  || extension == L"jpg"  || extension == L"JPG") {
			return GUID_ContainerFormatJpeg;
		}
		else if (extension == L"tif"  || extension == L"TIF"
			  || extension == L"tiff" || extension == L"TIFF") {
			return GUID_ContainerFormatTiff;
		}
		else if (extension == L"gif"  || extension == L"GIF") {
			return GUID_ContainerFormatGif;
		}
		else if (extension == L"bmp"  || extension == L"BMP"
			  || extension == L"dib"  || extension == L"DIB") {
			return GUID_ContainerFormatBmp;
		}
		else if (extension == L"ico"  || extension == L"ICO") {
			return GUID_ContainerFormatIco;
		}
		else if (extension == L"hdp"  || extension == L"HDP"
			  || extension == L"wdp"  || extension == L"WDP"
			  || extension == L"jxr"  || extension == L"JXR") {
			return GUID_ContainerFormatWmp;
		}
		else {
			return GUID_NULL;
		}
	}

	void ExportTextureToFile(const wstring &fname,
		ID3D11DeviceContext4 *device_context,
		ID3D11Resource *texture) {

		Assert(device_context);
		Assert(texture);

		const wstring extension = GetFileExtension(fname);

		if (extension == L"dds" || extension == L"DDS") {
			const HRESULT result = DirectX::SaveDDSTextureToFile(
				device_context, texture, fname.c_str());
			ThrowIfFailed(result, "Texture exporting failed: %08X.", result);
		}
		else {

			const GUID format = GetGUIDContainerFormat(extension);
			ThrowIfFailed((GUID_NULL != format), 
				"Unknown image file extension: %ls", fname.c_str());

			const HRESULT result = DirectX::SaveWICTextureToFile(
				device_context, texture, format, fname.c_str());
			ThrowIfFailed(result, "Texture exporting failed: %08X.", result);
		}
	}
}