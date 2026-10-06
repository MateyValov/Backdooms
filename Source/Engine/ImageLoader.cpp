#include "ImageLoader.h"

#include <cstdio>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#ifndef GL_CLAMP_TO_EDGE
#define GL_CLAMP_TO_EDGE 0x812F   // Windows' gl.h is OpenGL 1.1 and lacks this
#endif

GLuint tilesetTex = 0;
int texW = 0;
int texH = 0;
int tileCount = 1;

static const char* TILESET_PATH = "Textures/Tileset.bmp";
static const char* ITEMS_PATH = "Textures/Items.bmp";

static unsigned char* loadPixels(const char* path, int* w, int* h)
{
	int channels;
	// Always request 4 channels (RGBA) so the upload code never has to care
	// whether the file was 24-bit or 32-bit.
	return stbi_load(path, w, h, &channels, 4);
}

bool loadTileset()
{
	int w = 0, h = 0;
	unsigned char* pixels = nullptr;

	pixels = loadPixels(TILESET_PATH, &w, &h);
	if (!pixels)
	{
		printf("Could not find Textures/Tileset.bmp. Looked in:\n");
		return false;
	}

	texW = w;
	texH = h;
	tileCount = h / w;
	if (tileCount < 1) { tileCount = 1; }
	if (h % w != 0)
	{
		printf("Warning: tileset height (%d) is not a multiple of its width (%d); "
			"tiles will be cut off.\n", h, w);
	}

	if (!tilesetTex) { glGenTextures(1, &tilesetTex); }
	glBindTexture(GL_TEXTURE_2D, tilesetTex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);   // crisp pixels
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

	stbi_image_free(pixels);
	return true;
}