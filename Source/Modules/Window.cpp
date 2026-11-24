#include "Window.h"

#include "Architecture/Level.h"


void Window::InitWindow(int width, int height)
{
	fullscreen = Screenspace(width, height);
}

void Window::RenderScene(Level* level)
{
	if (level == nullptr)
	{
		return;
	}

	size_t count = level->GetCameras().size();
	for (size_t i = 0; i < count; ++i)
	{
		if (level->GetCameras()[i] == nullptr)
		{
			continue;
		}
		level->GetCameras()[i]->DrawLevel();
	}
}

void Window::RenderViewports(Level* level)
{
	if (level == nullptr)
	{
		return;
	}

	size_t count = level->GetViewports().size();
	for (size_t i = 0; i < count; ++i)
	{
		if (level->GetViewports()[i] == nullptr)
		{
			continue;
		}
		level->GetViewports()[i]->UpdateViewport();
	}
}

Screenspace Window::Fullscreen()
{
	return fullscreen;
}

Screenspace Window::TopHalf()
{
	return Screenspace(fullscreen.TopLeft, fullscreen.x(), fullscreen.y() / 2);
}

Screenspace Window::BottomHalf()
{
	return Screenspace(Vector2(fullscreen.TopLeft.x, fullscreen.y() / 2), fullscreen.BottomRight);
}

Screenspace Window::LeftHalf()
{
	return Screenspace(fullscreen.TopLeft, fullscreen.x() / 2, fullscreen.y());
}

Screenspace Window::RightHalf()
{
	return Screenspace(Vector2(fullscreen.x() / 2, fullscreen.TopLeft.y), fullscreen.BottomRight);
}

Screenspace Window::TopLeft()
{
	return Screenspace(fullscreen.TopLeft, fullscreen.BottomRight / 2);
}

Screenspace Window::TopRight()
{
	return Screenspace(Vector2(fullscreen.x() / 2, fullscreen.TopLeft.y), fullscreen.x() / 2, fullscreen.y() / 2);
}

Screenspace Window::BottomLeft()
{
	return Screenspace(Vector2(fullscreen.TopLeft.x, fullscreen.y() / 2), fullscreen.x() / 2, fullscreen.y() / 2);
}

Screenspace Window::BottomRight()
{
	return Screenspace(fullscreen.BottomRight / 2, fullscreen.BottomRight);
}
