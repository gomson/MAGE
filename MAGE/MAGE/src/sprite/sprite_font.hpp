#pragma once

//-----------------------------------------------------------------------------
// Engine Includes
//-----------------------------------------------------------------------------
#pragma region

#include "resource\resource.hpp"
#include "memory\memory.hpp"
#include "rendering\rendering.hpp"
#include "sprite\sprite_font_output.hpp"
#include "sprite\sprite_font_descriptor.hpp"
#include "sprite\sprite_transform.hpp"
#include "sprite\sprite_batch.hpp"
#include "sprite\sprite_effects.hpp"
#include "material\color.hpp"

#pragma endregion

//-----------------------------------------------------------------------------
// Engine Declarations
//-----------------------------------------------------------------------------
namespace mage {

	class SpriteFont : public Resource {

	public:

		SpriteFont(ComPtr< ID3D11Device2 > device, const wstring &fname, const SpriteFontDescriptor &desc);
		virtual ~SpriteFont() = default;

		void DrawString(SpriteBatch &sprite_batch, const wchar_t *text, const SpriteTransform &transform,
			XMVECTOR color = Colors::White, SpriteEffects effects = SpriteEffects_None, float layer_depth = 0.0f) const;

		XMVECTOR MeasureString(const wchar_t *text) const;
		RECT MeasureDrawBounds(const wchar_t *text, const XMFLOAT2 &position) const;
		
		float GetLineSpacing() const;
		void SetLineSpacing(float spacing);
		wchar_t GetDefaultCharacter() const;
		void SetDefaultCharacter(wchar_t character);
		bool ContainsCharacter(wchar_t character) const;
		const Glyph *GetGlyph(wchar_t character) const;

	private:

		SpriteFont(const SpriteFont &font) = delete;
		SpriteFont &operator=(const SpriteFont &font) = delete;

		HRESULT InitializeSpriteFont(const SpriteFontOutput &output);

		ComPtr< ID3D11ShaderResourceView > m_texture;
		vector < Glyph > m_glyphs;
		const Glyph *m_default_glyph;
		float m_line_spacing;
	};
}