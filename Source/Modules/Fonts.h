#pragma once

#include "Structs/Transform.h"

#include <GL/glut.h>
#include "stb_truetype.h"
#include <string>

namespace Fonts
{
	struct Font
	{
		static constexpr int FirstChar = 32;
		static constexpr int CharCount = 96;
		static constexpr int AtlasSize = 512;

		stbtt_bakedchar glyphs[CharCount] = {};
		GLuint texture = 0;
		float bakeHeight = 32.0f;
		float ascent = 0.0f;
		float lineHeight = 0.0f;

		bool Load(const char* ttfPath, float pixelHeight = 32.0f);
		void Unload();
		bool IsLoaded() const;
		Vector2 Measure(const std::string& text, float scale) const;

		bool GetGlyphQuad(unsigned char c, float& penX, float baselineY, stbtt_aligned_quad& quad) const;
	};

	Font& GetBase();
	bool InitFont(const char* fileName, float pixelHeight = 32.0f);
	void ShutdownFont();
	Vector2 MeasureString(const std::string& text, float scale);
}
