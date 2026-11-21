#include "Renderer.h"

Renderer::Renderer(Color color)
{
	this->color = color;
}

void Renderer::SetColor(Color color)
{
	this->color = color;
}

Color Renderer::GetColor() const
{
	return color;
}