#pragma once

#include "Component.h"

#include "Structs/Transform.h"
#include "Structs/Image.h"

class Renderer : public Component
{
protected:
	Color color = Color();

public:
	Renderer() = default;
	Renderer(Color color);
	virtual ~Renderer() = default;

	virtual int GetTextureId() const = 0;
	virtual int GetTextureIdForPosition(Vector2 viewerPosition) const = 0;

	void SetColor(Color color);
	Color GetColor() const;
};
