#pragma once

#include "MathFuncs.h"

struct Vector2
{
	float x = 0.f;
	float y = 0.f;

	Vector2() = default;
	Vector2(float xy) : x(xy), y(xy) {}
	Vector2(float x, float y) : x(x), y(y) {}

	Vector2 operator+(const Vector2& o) const { return Vector2(x + o.x, y + o.y); }
	Vector2 operator-(const Vector2& o) const { return Vector2(x - o.x, y - o.y); }
	Vector2 operator*(float s) const { return Vector2(x * s, y * s); }
	Vector2 operator*(const Vector2& o) const { return Vector2(x * o.x, y * o.y); }
	Vector2 operator/(float s) const { return Vector2(x / s, y / s); }
	Vector2 operator-() const { return Vector2(-x, -y); }

	Vector2& operator+=(const Vector2& o) { x += o.x; y += o.y; return *this; }
	Vector2& operator-=(const Vector2& o) { x -= o.x; y -= o.y; return *this; }
	Vector2& operator*=(float s) { x *= s; y *= s; return *this; }

	bool operator==(const Vector2& o) const { return x == o.x && y == o.y; }
	bool operator!=(const Vector2& o) const { return !(*this == o); }

	void normalize()
	{
		float m = magnitude();
		if (m < 0.0001f) 
		{
			x = 0.f;
			y = 0.f; 
			return; 
		}

		x /= m;
		y /= m;
	}

	float magnitude() const
	{
		return sqrtf(x * x + y * y);
	}

	static float distance(const Vector2& a, const Vector2& b)
	{
		return sqrtf((b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y));
	}

	static float dot(const Vector2& a, const Vector2& b)
	{
		return a.x * b.x + a.y * b.y;
	}

	static float cross(const Vector2& a, const Vector2& b)
	{
		return a.x * b.y - a.y * b.x;
	}

	static float angle(const Vector2& a, const Vector2& b)
	{
		float dot = Vector2::dot(a, b);
		float cross = Vector2::cross(a, b);
		return Math::radToDeg(atan2f(fabsf(cross), dot));
	}

	static float angleRad(const Vector2& a, const Vector2& b)
	{
		return Math::degToRad(angle(a, b));
	}

	static float angleSigned(const Vector2& a, const Vector2& b)
	{
		float dot = Vector2::dot(a, b);
		float cross = Vector2::cross(a, b);
		return Math::radToDeg(atan2f(cross, dot));
	}

	static float angleRadSigned(const Vector2& a, const Vector2& b)
	{
		return Math::degToRad(angleSigned(a, b));
	}
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
		return Vector2(cos(Math::degToRad(angle)), -sin(Math::degToRad(angle)));
	}

	Vector2 GetRight() const
	{
		return Vector2(sin(Math::degToRad(angle)), cos(Math::degToRad(angle)));
	}

	static float angleToSigned(const Transform& a, const Vector2& b)
	{
		Vector2 forward = a.GetForward();
		Vector2 right = a.GetRight();
		Vector2 toTarget(b.x - a.position.x, b.y - a.position.y);

		float ahead = toTarget.x * forward.x + toTarget.y * forward.y;
		float side = toTarget.x * right.x + toTarget.y * right.y;

		return Math::radToDeg(atan2f(-side, ahead));
	}

	bool operator==(const Transform& o) const { return position == o.position && angle == o.angle; }
	bool operator!=(const Transform& o) const { return !(*this == o); }

	Transform operator+(const Transform& o) const { return Transform(Vector2(position.x + o.position.x, position.y + o.position.y), angle + o.angle); }
	Transform operator-(const Transform& o) const { return Transform(Vector2(position.x - o.position.x, position.y - o.position.y), angle - o.angle); }

	Transform& operator+=(const Transform& o) { position.x += o.position.x; position.y += o.position.y; angle += o.angle; return *this; }
	Transform& operator-=(const Transform& o) { position.x -= o.position.x; position.y -= o.position.y; angle -= o.angle; return *this; }
};