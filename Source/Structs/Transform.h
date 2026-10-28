#pragma once

#include "MathFuncs.h"

struct Vector2
{
	float x = 0.f;
	float y = 0.f;

	Vector2() = default;
	Vector2(float x, float y) : x(x), y(y) {}

	Vector2 operator+(const Vector2& o) const { return Vector2(x + o.x, y + o.y); }
	Vector2 operator-(const Vector2& o) const { return Vector2(x - o.x, y - o.y); }
	Vector2 operator*(float s) const { return Vector2(x * s, y * s); }
	Vector2 operator*(const Vector2& o) const { return Vector2(x * o.x, y * o.y); }
	Vector2 operator-() const { return Vector2(-x, -y); }

	Vector2& operator+=(const Vector2& o) { x += o.x; y += o.y; return *this; }
	Vector2& operator-=(const Vector2& o) { x -= o.x; y -= o.y; return *this; }
	Vector2& operator*=(float s) { x *= s; y *= s; return *this; }

	bool operator==(const Vector2& o) const { return x == o.x && y == o.y; }
	bool operator!=(const Vector2& o) const { return !(*this == o); }
};

inline Vector2 operator*(float s, const Vector2& v) { return v * s; }

struct Vector3
{
	float x = 0.f;
	float y = 0.f;
	float z = 0.f;

	Vector3() = default;
	Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
};

struct Transform
{
	Vector2 position = Vector2();
	float angle = 0.f;

	Transform() = default;
	Transform(Vector2 position, float angle) : position(position), angle(angle) {}

	Vector2 GetForward() const
	{
		return Vector2(cos(degToRad(angle)), -sin(degToRad(angle)));
	}

	Vector2 GetRight() const
	{
		return Vector2(sin(degToRad(angle)), cos(degToRad(angle)));
	}

	bool operator==(const Transform& o) const { return position == o.position && angle == o.angle; }
	bool operator!=(const Transform& o) const { return !(*this == o); }
};