#pragma once
#include <GL/glut.h>
#include <string>

#include "../Structs/Image.h"

using namespace std;

namespace Textures
{
	bool LoadTextures();

	namespace Atlas
	{
#define ATLAS_COLUMNS 8
#define ATLAS_ROWS 8
#define ATLAS_COUNT (ATLAS_COLUMNS * ATLAS_ROWS)

		bool Load();

		const Image& GetImage();     
		int CellSize();              
		Rect Cell(int index); 

		void Sample(int index, float u, float v, unsigned char rgba[4]);
	}

	namespace GUI
	{
		bool Load();

		const Image& Get(const string& path);
	}

	bool LoadRgba(const char* path, int& width, int& height, std::vector<unsigned char>& rgba);
	bool CreateImage(Image& out, int width, int height, const unsigned char* rgba, bool keepPixels = false, bool smooth = false);
	bool LoadImage(const char* path, Image& out, bool keepPixels = false, bool smooth = false);
};