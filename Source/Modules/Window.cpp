#include "Window.h"

void Window::InitWindow(int width, int height)
{
	fullscreen = Screenspace(width, height);
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
