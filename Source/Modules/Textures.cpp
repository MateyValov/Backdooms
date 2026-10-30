#include "Textures.h"

#include <algorithm>
#include <cctype>
#include <cstdio>
#include <filesystem>
#include <unordered_map>
#include <unordered_set>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#ifndef GL_CLAMP_TO_EDGE
#define GL_CLAMP_TO_EDGE 0x812F   // Windows' gl.h is OpenGL 1.1 and lacks this
#endif

static const char* ATLAS_PATH = "Atlas/Atlas.png";
static const char* GUI_PATH = "GUI";

static Image AtlasImage;      
static int AtlasCell = 16;

static unordered_map<std::string, Image> guiImages;

static Image MissingImage;   


static string lower(string s)
{
	transform(s.begin(), s.end(), s.begin(), [](unsigned char c) { return (char)tolower(c); });
	return s;
}

static string normalize(string path)
{
	replace(path.begin(), path.end(), '\\', '/');
	return lower(path);
}

static bool isImageFile(const filesystem::path& p)
{
	string ext = lower(p.extension().string());
	return ext == ".png" || ext == ".bmp" || ext == ".tga" || ext == ".jpg" || ext == ".jpeg";
}

static bool findTexturesRoot(filesystem::path& out)
{
	for (const char* candidate : { "Textures", "../Textures", "../../Textures" })
	{
		if (filesystem::is_directory(candidate)) { out = candidate; return true; }
	}
	return false;
}

static void fillChecker(vector<unsigned char>& rgba, int width, int x0, int y0, int size)
{
	int block = std::max(1, size / 2);
	for (int y = 0; y < size; ++y)
	{
		for (int x = 0; x < size; ++x)
		{
			bool magenta = ((x / block) + (y / block)) % 2 == 0;
			unsigned char* p = &rgba[((y0 + y) * width + x0 + x) * 4];
			p[0] = magenta ? 255 : 0; p[1] = 0; p[2] = magenta ? 255 : 0; p[3] = 255;
		}
	}
}

static void createMissingTextures()
{
	const int SIZE = 16;
	vector<unsigned char> rgba(SIZE * SIZE * 4);
	fillChecker(rgba, SIZE, 0, 0, SIZE);
	Textures::CreateImage(MissingImage, SIZE, SIZE, rgba.data(), false, false);
}

static unsigned char* loadPixels(const char* path, int* w, int* h)
{
	static const char* PREFIXES[] = { "", "../", "../../" };
	string report;
	for (const char* prefix : PREFIXES)
	{
		string full = string(prefix) + path;
		int channels;
		unsigned char* px = stbi_load(full.c_str(), w, h, &channels, 4);
		if (px) { return px; }
		report += "  " + full + " -> " + stbi_failure_reason() + "\n";
	}
	printf("Could not load '%s':\n%s", path, report.c_str());
	return nullptr;
}

// ============================================================================================
// LOW LEVEL
// ============================================================================================
namespace Textures
{
	bool LoadRgba(const char* path, int& width, int& height, std::vector<unsigned char>& rgba)
	{
		unsigned char* pixels = loadPixels(path, &width, &height);
		if (!pixels) { return false; }
		rgba.assign(pixels, pixels + (size_t)width * height * 4);
		stbi_image_free(pixels);
		return true;
	}

	bool CreateImage(Image& out, int width, int height, const unsigned char* rgba, bool keepPixels, bool smooth)
	{
		if (width <= 0 || height <= 0 || !rgba) { return false; }

		if (!out.texture) { glGenTextures(1, &out.texture); }
		glBindTexture(GL_TEXTURE_2D, out.texture);
		GLint filter = smooth ? GL_LINEAR : GL_NEAREST;
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, rgba);

		if (keepPixels) { out.pixels.assign(rgba, rgba + (size_t)width * height * 4); }
		else { out.pixels.clear(); out.pixels.shrink_to_fit(); }

		out.width = width;
		out.height = height;
		return true;
	}

	bool LoadImage(const char* path, Image& out, bool keepPixels, bool smooth)
	{
		int w = 0, h = 0;
		vector<unsigned char> rgba;
		if (!LoadRgba(path, w, h, rgba)) { return false; }
		return CreateImage(out, w, h, rgba.data(), keepPixels, smooth);
	}
}

// ============================================================================================
// ATLAS
// ============================================================================================
namespace Textures {
	namespace Atlas
	{
		bool Load()
		{
			int fileW = 0, fileH = 0;
			std::vector<unsigned char> file;
			bool ok = false;

			filesystem::path root;
			if (findTexturesRoot(root))
			{
				std::string path = (root / ATLAS_PATH).generic_string();
				ok = LoadRgba(path.c_str(), fileW, fileH, file) && fileW >= ATLAS_COLUMNS;
			}
			else
			{
				printf("Atlas: could not find a 'Textures' folder (looked here, one and two folders up).\n");
			}

			if (ok)
			{
				if (fileW != fileH || fileW % ATLAS_COLUMNS != 0)
				{
					printf("Warning: atlas is %dx%d. It should be square, with a width divisible by %d.\n", fileW, fileH, (int)ATLAS_COLUMNS);
				}
				AtlasCell = fileW / ATLAS_COLUMNS;
			}
			else
			{
				printf("Atlas: using a placeholder, every texture will be the missing-texture checkerboard.\n");
				AtlasCell = 16;
			}

			const int width = ATLAS_COLUMNS * AtlasCell;
			const int gridHeight = ATLAS_ROWS * AtlasCell;
			const int height = gridHeight + AtlasCell;          // + one hidden row holding the missing texture
			std::vector<unsigned char> pixels((size_t)width * height * 4, 0);

			if (ok)
			{
				for (int y = 0; y < std::min(fileH, gridHeight); ++y)
					std::copy_n(&file[(size_t)y * fileW * 4], (size_t)std::min(fileW, width) * 4, &pixels[(size_t)y * width * 4]);
			}
			else
			{
				for (int i = 0; i < ATLAS_COUNT; ++i) { fillChecker(pixels, width, (i % ATLAS_COLUMNS) * AtlasCell, (i / ATLAS_COLUMNS) * AtlasCell, AtlasCell); }
			}
			fillChecker(pixels, width, 0, gridHeight, AtlasCell);   // the missing texture, outside the numbered grid
			CreateImage(AtlasImage, width, height, pixels.data(), true, false);   // crisp: neighbouring textures can't bleed
			return ok;
		}

		const Image& GetImage() 
		{ 
			return AtlasImage; 
		}

		int CellSize() 
		{ 
			return AtlasCell; 
		}

		Rect Cell(int index)
		{
			float size = (float)AtlasCell;
			if (index < 0 || index >= ATLAS_COUNT)
			{
				static std::unordered_set<int> warned;
				if (warned.insert(index).second) { printf("Atlas: texture %d does not exist (valid: 0 to %d).\n", index, (int)ATLAS_COUNT - 1); }
				return Rect(0, (float)(ATLAS_ROWS * AtlasCell), size, size);   // the hidden missing texture
			}
			return Rect((float)((index % ATLAS_COLUMNS) * AtlasCell), (float)((index / ATLAS_COLUMNS) * AtlasCell), size, size);
		}

		void Sample(int index, float u, float v, unsigned char rgba[4])
		{
			if (AtlasImage.pixels.empty()) { rgba[0] = 255; rgba[1] = 0; rgba[2] = 255; rgba[3] = 255; return; }
			Rect c = Cell(index);
			int tx = std::min(std::max((int)(u * AtlasCell), 0), AtlasCell - 1);
			int ty = std::min(std::max((int)(v * AtlasCell), 0), AtlasCell - 1);
			const unsigned char* src = &AtlasImage.pixels.data()[(((int)c.y + ty) * AtlasImage.width + (int)c.x + tx) * 4];
			rgba[0] = src[0]; rgba[1] = src[1]; rgba[2] = src[2]; rgba[3] = 255;
		}
	}
}

template <typename T>
static void warnMissing(const char* kind, const std::string& path, const std::unordered_map<std::string, T>& map)
{
	static std::unordered_set<std::string> warned;
	if (!warned.insert(std::string(kind) + path).second) { return; }   // already told you about this one
	printf("Textures: no GUI %s '%s'. Available:", kind, path.c_str());
	for (const auto& entry : map) { printf(" %s", entry.first.c_str()); }
	printf("\n");
}

namespace Textures {
	namespace GUI
	{
		bool Load()
		{
			filesystem::path root;
			if (!findTexturesRoot(root))
			{
				printf("GUI: could not find a 'Textures' folder (looked here, one and two folders up).\n");
				return false;
			}
			filesystem::path dir = root / GUI_PATH;
			if (!filesystem::is_directory(dir))
			{
				printf("GUI: folder '%s' not found.\n", dir.generic_string().c_str());
				return false;
			}

			bool ok = true;
			int loaded = 0;
			error_code ec;
			for (const filesystem::directory_entry& entry : filesystem::recursive_directory_iterator(dir, ec))
			{
				if (!entry.is_regular_file() || !isImageFile(entry.path())) { continue; }

				string key = normalize(filesystem::relative(entry.path(), root).generic_string()); 
				string file = entry.path().generic_string();

				bool isNew = guiImages.find(key) == guiImages.end();
				Image& image = guiImages[key];
				if (!LoadImage(file.c_str(), image, false, false))
				{
					ok = false;
					if (isNew) { guiImages.erase(key); }   // never erase an entry someone may hold a reference to
					continue;
				}
				++loaded;
			}
			printf("GUI: %d images loaded from '%s'.\n", loaded, dir.generic_string().c_str());
			return ok;
		}

		const Image& Get(const string& path)
		{
			auto it = guiImages.find(normalize(path));
			if (it != guiImages.end()) { return it->second; }
			warnMissing("image", path, guiImages);
			return MissingImage;
		}
	}
}

// ============================================================================================
bool Textures::LoadTextures()
{
	if (!MissingImage.texture) 
	{ 
		createMissingTextures(); 
	}

	bool ok = Atlas::Load() && GUI::Load();
	return ok;
}
