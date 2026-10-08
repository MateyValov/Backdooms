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
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

	tilesetPixels.assign(pixels, pixels + (size_t)w * h * 4);

	stbi_image_free(pixels);
	return true;
}

bool loadImage(const char* path, Image& out)
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

void drawImage(const Image& img, float x, float y, float w, float h)
{
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindTexture(GL_TEXTURE_2D, img.texture);
	glColor3f(1, 1, 1);   // white = show the image's own colours unchanged

	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex2f(x, y);
	glTexCoord2f(1, 0); glVertex2f(x + w, y);
	glTexCoord2f(1, 1); glVertex2f(x + w, y + h);
	glTexCoord2f(0, 1); glVertex2f(x, y + h);
	glEnd();

	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
}
