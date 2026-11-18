#include "Fonts.h"

#include <algorithm>
#include <fstream>
#include <iterator>
#include <vector>

#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"

static Fonts::Font BaseFont;
static const std::string FontsPath = "Assets/Fonts";

bool Fonts::Font::Load(const char* ttfPath, float pixelHeight)
{
	std::ifstream file(ttfPath, std::ios::binary);
	if (!file) return false;
	std::vector<unsigned char> ttf((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

	std::vector<unsigned char> bitmap(AtlasSize * AtlasSize);
	if (stbtt_BakeFontBitmap(ttf.data(), 0, pixelHeight, bitmap.data(),
		AtlasSize, AtlasSize, FirstChar, CharCount, glyphs) <= 0)
		return false;

	stbtt_fontinfo info;
	if (!stbtt_InitFont(&info, ttf.data(), stbtt_GetFontOffsetForIndex(ttf.data(), 0)))
		return false;

	int a, d, gap;
	stbtt_GetFontVMetrics(&info, &a, &d, &gap);
	float s = stbtt_ScaleForPixelHeight(&info, pixelHeight);

	bakeHeight = pixelHeight;
	ascent = a * s;
	lineHeight = (a - d + gap) * s;

	Unload();
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, AtlasSize, AtlasSize, 0, GL_ALPHA, GL_UNSIGNED_BYTE, bitmap.data());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return true;
}

void Fonts::Font::Unload()
{
	if (texture)
	{
		glDeleteTextures(1, &texture);
		texture = 0;
	}
}

bool Fonts::Font::IsLoaded() const
{
	return texture != 0;
}

Vector2 Fonts::Font::Measure(const std::string& text, float scale) const
{
	if (!IsLoaded()) return Vector2(0, 0);

	float penX = 0, maxW = 0;
	int lines = 1;
	for (unsigned char c : text)
	{
		if (c == '\n') { maxW = std::max(maxW, penX); penX = 0; ++lines; continue; }
		if (c < FirstChar || c >= FirstChar + CharCount) continue;
		penX += glyphs[c - FirstChar].xadvance;
	}
	maxW = std::max(maxW, penX);
	return Vector2(maxW * scale, lines * lineHeight * scale);
}

bool Fonts::Font::GetGlyphQuad(unsigned char c, float& penX, float baselineY, stbtt_aligned_quad& quad) const
{
	if (c < FirstChar || c >= FirstChar + CharCount) return false;
	float y = baselineY;
	stbtt_GetBakedQuad(glyphs, AtlasSize, AtlasSize, c - FirstChar, &penX, &y, &quad, 1);
	return true;
}

Fonts::Font& Fonts::GetBase() 
{ 
	return BaseFont; 
}

bool Fonts::InitFont(const char* fileName, float pixelHeight)
{ 
	if (!fileName || !*fileName)
	{
		return false;
	}

	const std::string fullPath = (FontsPath + '/' + fileName);
	return BaseFont.Load(fullPath.c_str(), pixelHeight);
}

void Fonts::ShutdownFont() 
{ 
	BaseFont.Unload(); 
}

Vector2 Fonts::MeasureString(const std::string& t, float s) 
{ 
	return BaseFont.Measure(t, s); 
}