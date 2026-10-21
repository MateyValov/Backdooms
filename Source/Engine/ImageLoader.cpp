#include "ImageLoader.h"

#include <cstdio>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#ifndef GL_CLAMP_TO_EDGE
#define GL_CLAMP_TO_EDGE 0x812F   // Windows' gl.h is OpenGL 1.1 and lacks this
#endif

vector<unsigned char> tilesetPixels;

GLuint tilesetTex = 0;
int texW = 0;
int texH = 0;
int tileCount = 1;

static const char* TILESET_PATH = "Textures/Tileset.bmp";
static const char* ITEMS_PATH = "Textures/Items.bmp";
static const char* UI_PATH = "Textures/UI.bmp";

static unsigned char* loadPixels(const char* path, int* w, int* h)
{
	int channels;
	return stbi_load(path, w, h, &channels, 4);
}

bool ImageLoader::loadTileset()
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
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

	tilesetPixels.assign(pixels, pixels + (size_t)w * h * 4);

	stbi_image_free(pixels);
	return true;
}

bool ImageLoader::loadTileset(const char* path, int tileCount, Tileset& out)
{
	if (tileCount < 1) { tileCount = 1; }
	if (!loadImage(path, out.image)) { return false; }

	out.tileCount = tileCount;
	out.tileW = out.image.width;
	out.tileH = out.image.height / tileCount;
	if (out.image.height % tileCount != 0)
	{
		printf("Warning: '%s' is %d pixels tall, which is not a multiple of %d tiles.\n",
			path, out.image.height, tileCount);
	}

	// Crisp pixels, same as the wall tileset. With GL_NEAREST neighbouring tiles
	// can never bleed into each other.
	glBindTexture(GL_TEXTURE_2D, out.image.texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	return true;
}

bool ImageLoader::loadImage(const char* path, Image& out)
{
	int w, h, channels;
	unsigned char* pixels = stbi_load(path, &w, &h, &channels, 4);
	if (!pixels)
	{
		printf("Could not load image '%s': %s\n", path, stbi_failure_reason());
		return false;
	}

	if (!out.texture) { glGenTextures(1, &out.texture); }
	glBindTexture(GL_TEXTURE_2D, out.texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);   // smooth if it ever gets scaled
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);   // use GL_NEAREST for pixel art
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
	stbi_image_free(pixels);

	out.width = w;
	out.height = h;
	return true;
}

void ImageLoader::drawImage(const Image& image, Vector2 position, float widthOverride, float heightOverride, Color tint)
{
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindTexture(GL_TEXTURE_2D, image.texture);
	glColor3f(tint.r, tint.g, tint.b);

	float w = widthOverride != 0 ? widthOverride : image.width;
	float h = heightOverride != 0 ? heightOverride : image.height;

	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex2f(position.x, position.y);
	glTexCoord2f(1, 0); glVertex2f(position.x + w, position.y);
	glTexCoord2f(1, 1); glVertex2f(position.x + w, position.y + h);
	glTexCoord2f(0, 1); glVertex2f(position.x, position.y + h);
	glEnd();

	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
}

void ImageLoader::drawNumber(int value, Vector2 position, Color tint)
{
}
