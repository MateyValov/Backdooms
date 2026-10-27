#include "ImageLoader.h"

#include <cstdio>
#include <string>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#ifndef GL_CLAMP_TO_EDGE
#define GL_CLAMP_TO_EDGE 0x812F   // Windows' gl.h is OpenGL 1.1 and lacks this
#endif

//vector<unsigned char> tilesetPixels;
//
//GLuint tilesetTex = 0;
//int texW = 0;
//int texH = 0;
//int tileCount = 1;

static const char* TILESET_PATH = "Textures/Tileset.bmp";
static const char* ITEMS_PATH = "Textures/Items.bmp";
static const char* UI_PATH = "Textures/UI.bmp";

static unsigned char* loadPixels(const char* path, int* w, int* h)
{
	int channels;
	return stbi_load(path, w, h, &channels, 4);

	static const char* PREFIXES[] = { "", "../", "../../" };
	std::string report;
	for (const char* prefix : PREFIXES)
	{
		std::string full = std::string(prefix) + path;
		int channels;
		unsigned char* px = stbi_load(full.c_str(), w, h, &channels, 4);   // always RGBA
		if (px) { return px; }
		report += "  " + full + " -> " + stbi_failure_reason() + "\n";
	}
	printf("Could not load '%s':\n%s", path, report.c_str());
	return nullptr;
}

bool loadImage(const char* path, Image& out, bool keepPixels)
{
	int w = 0, h = 0;
	unsigned char* pixels = loadPixels(path, &w, &h);
	if (!pixels) { return false; }

	if (!out.texture) { glGenTextures(1, &out.texture); }
	glBindTexture(GL_TEXTURE_2D, out.texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);   // smooth if it ever gets scaled
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);   // use GL_NEAREST for pixel art
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

	if (keepPixels) { out.pixels.assign(pixels, pixels + (size_t)w * h * 4); }
	else { out.pixels.clear(); out.pixels.shrink_to_fit(); }

	stbi_image_free(pixels);
	out.width = w;
	out.height = h;
	return true;
}

bool loadTileset(const char* path, int tileCount, Tileset& out, bool keepPixels)
{
	if (!loadImage(path, out.image, keepPixels)) { return false; }

	if (tileCount <= 0) { tileCount = out.image.height / out.image.width; }   // square tiles
	if (tileCount < 1) { tileCount = 1; }

	out.tileCount = tileCount;
	out.tileW = out.image.width;
	out.tileH = out.image.height / tileCount;
	if (out.image.height % tileCount != 0)
	{
		printf("Warning: '%s' is %d pixels tall, which is not a multiple of %d tiles.\n",
			path, out.image.height, tileCount);
	}

	// Crisp pixels, so neighbouring tiles can never bleed into each other.
	glBindTexture(GL_TEXTURE_2D, out.image.texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	return true;
}
