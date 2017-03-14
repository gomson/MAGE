//-----------------------------------------------------------------------------
// Engine Includes
//-----------------------------------------------------------------------------
#pragma region

#include "material\mtl\mtl_reader.hpp"
#include "material\mtl\mtl_tokens.hpp"
#include "string\string_utils.hpp"
#include "logging\error.hpp"

#pragma endregion

//-----------------------------------------------------------------------------
// Engine Declarations
//-----------------------------------------------------------------------------
namespace mage {

	HRESULT MTLReader::ReadLine(char *line) {
		m_context = nullptr;
		const char *token = strtok_s(line, GetDelimiters().c_str(), &m_context);

		if (!token || token[0] == MAGE_MTL_COMMENT_CHAR) {
			return S_OK;
		}

		if (str_equals(token, MAGE_MTL_TOKEN_MATERIAL_DECLARATION)) {
			ReadMTLMaterialName();
		}
		else if (str_equals(token, MAGE_MTL_TOKEN_TRANSMISSION_FILTER)) {
			ReadMTLTransmissionFilter();
		}
		else if (str_equals(token, MAGE_MTL_TOKEN_AMBIENT_REFLECTIVITY)) {
			ReadMTLAmbientReflectivity();
		}
		else if (str_equals(token, MAGE_MTL_TOKEN_DIFFUSE_REFLECTIVITY)) {
			ReadMTLDiffuseReflectivity();
		}
		else if (str_equals(token, MAGE_MTL_TOKEN_SPECULAR_REFLECTIVITY)) {
			ReadMTLSpecularReflectivity();
		}
		else if (str_equals(token, MAGE_MTL_TOKEN_SPECULAR_EXPONENT)) {
			ReadMTLSpecularExponent();
		}
		else if (str_equals(token, MAGE_MTL_TOKEN_DISSOLVE)) {
			ReadMTLDissolve();
		}
		else if (str_equals(token, MAGE_MTL_TOKEN_OPTICAL_DENSITY)) {
			ReadMTLOpticalDensity();
		}
		else if (str_equals(token, MAGE_MTL_TOKEN_AMBIENT_REFLECTIVITY_MAP)) {
			ReadMTLAmbientReflectivityTexture();
		}
		else if (str_equals(token, MAGE_MTL_TOKEN_DIFFUSE_REFLECTIVITY_MAP)) {
			ReadMTLDiffuseReflectivityTexture();
		}
		else if (str_equals(token, MAGE_MTL_TOKEN_SPECULAR_REFLECTIVITY_MAP)) {
			ReadMTLSpecularReflectivityTexture();
		}
		else if (str_equals(token, MAGE_MTL_TOKEN_SPECULAR_EXPONENT_MAP)) {
			ReadMTLSpecularExponentTexture();
		}
		else if (str_equals(token, MAGE_MTL_TOKEN_DISSOLVE_MAP)) {
			ReadMTLDissolveTexture();
		}
		else if (str_equals(token, MAGE_MTL_TOKEN_DECAL_MAP)) {
			ReadMTLDecalTexture();
		}
		else if (str_equals(token, MAGE_MTL_TOKEN_DISPLACEMENT_MAP)) {
			ReadMTLDisplacementTexture();
		}
		else if (str_equals(token, MAGE_MTL_TOKEN_BUMP_MAP)) {
			ReadMTLBumpTexture();
		}
		else {
			Warning("%ls: line %u: unsupported keyword token: %s.", GetFilename().c_str(), GetCurrentLineNumber(), token);
		}

		ReadLineRemaining();

		return S_OK;
	}

	void MTLReader::ReadMTLMaterialName() {
		const string material_name = ReadString();
		m_material_buffer.push_back(Material(material_name));
	}

	void MTLReader::ReadMTLTransmissionFilter() {
		m_material_buffer.back().m_transmission_filter = ReadMTLSpectrum();
	}

	void MTLReader::ReadMTLAmbientReflectivity() {
		m_material_buffer.back().m_ambient_reflectivity = ReadMTLSpectrum();
	}

	void MTLReader::ReadMTLDiffuseReflectivity() {
		m_material_buffer.back().m_diffuse_reflectivity = ReadMTLSpectrum();
	}

	void MTLReader::ReadMTLSpecularReflectivity() {
		m_material_buffer.back().m_specular_reflectivity = ReadMTLSpectrum();
	}

	void MTLReader::ReadMTLSpecularExponent() {
		m_material_buffer.back().m_specular_exponent = ReadFloat();
	}

	void MTLReader::ReadMTLDissolve() {
		if (HasChars()) {
			const char *token = ReadChars();
			Warning("%ls: line %u: unsupported keyword token: %s.", GetFilename().c_str(), GetCurrentLineNumber(), token);
		}

		m_material_buffer.back().m_dissolve = ReadFloat();
	}

	void MTLReader::ReadMTLOpticalDensity() {
		m_material_buffer.back().m_index_of_refraction = ReadFloat();
	}

	void MTLReader::ReadMTLAmbientReflectivityTexture() {
		m_material_buffer.back().m_ambient_reflectivity_texture = ReadMTLTexture();
	}

	void MTLReader::ReadMTLDiffuseReflectivityTexture() {
		m_material_buffer.back().m_diffuse_reflectivity_texture = ReadMTLTexture();
	}

	void MTLReader::ReadMTLSpecularReflectivityTexture() {
		m_material_buffer.back().m_specular_reflectivity_texture = ReadMTLTexture();
	}

	void MTLReader::ReadMTLSpecularExponentTexture() {
		m_material_buffer.back().m_specular_exponent_texture = ReadMTLTexture();
	}

	void MTLReader::ReadMTLDissolveTexture() {
		m_material_buffer.back().m_dissolve_texture = ReadMTLTexture();
	}

	void MTLReader::ReadMTLDecalTexture() {
		m_material_buffer.back().m_decal_texture = ReadMTLTexture();
	}

	void MTLReader::ReadMTLDisplacementTexture() {
		m_material_buffer.back().m_displacement_texture = ReadMTLTexture();
	}

	void MTLReader::ReadMTLBumpTexture() {
		m_material_buffer.back().m_bump_texture = ReadMTLTexture();
	}

	RGBSpectrum MTLReader::ReadMTLSpectrum() {
		if (!HasFloat()) {
			const char *str = ReadChars();

			// XYZ spectrum
			if (str_equals(str, MAGE_MTL_TOKEN_XYZ)) {
				const float x = ReadFloat();
				float y = x;
				float z = x;
				if (HasFloat()) {
					y = ReadFloat();
					z = ReadFloat();
				}
				return XYZSpectrum(RGBSpectrum(x, y, z));
			}
			
			// Custom spectrum
			if (str_equals(str, MAGE_MTL_TOKEN_SPECTRAL)) {
				Warning("%ls: line %u: unsupported custom spectrum.", GetFilename().c_str(), GetCurrentLineNumber());
				return RGBSpectrum();
			}

			Warning("%ls: line %u: unsupported spectrum: %s.", GetFilename().c_str(), GetCurrentLineNumber(), str);
			return RGBSpectrum();
		}

		// RGB spectrum
		const float x = ReadFloat();
		float y = x;
		float z = x;
		if (HasFloat()) {
			y = ReadFloat();
			z = ReadFloat();
		}
		return RGBSpectrum(x, y, z);
	}

	SharedPtr< Texture > MTLReader::ReadMTLTexture() {
		// "-options args" are not supported and are not allowed.
		const wstring texture_path = mage::GetPathName(GetFilename());
		const wstring texture_name = str_convert(ReadString());
		const wstring texture_fname = mage::GetFilename(texture_path, texture_name);
		return CreateTexture(texture_fname);
	}
}