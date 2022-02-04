#ifndef VECTOR2_H
#define VECTOR2_H

#include "core/string.h"

class Vector2i;

class Vector2 {
public:
	Vector2 abs() const;
	float angle() const;
	float angle_to(const Vector2 &b) const;
	float cross(const Vector2 &b) const;
	Vector2 clamped(float len) const;
	Vector2 direction_to(const Vector2 &b) const;
	float distance_to_squared(const Vector2 &b) const;
	float distance_to(const Vector2 &b) const;
	float dot(const Vector2 &b) const;
	bool is_equal_approx(const Vector2 &b) const;

	float length() const;
	float length_squared() const;

	Vector2 lerp(const Vector2 &b, const float t) const;

	Vector2 normalized() const;
	void normalize();

	void add(const Vector2 &b);
	void sub(const Vector2 &b);

	Vector2();
	Vector2(const Vector2 &b);
	Vector2(const float p_x, const float p_y);

	Vector2 &operator+=(const Vector2 &b);
	Vector2 operator+(const Vector2 &b) const;
	Vector2 &operator-=(const Vector2 &b);
	Vector2 operator-(const Vector2 &b) const;
	Vector2 &operator*=(const Vector2 &b);
	Vector2 operator*(const Vector2 &b) const;
	Vector2 &operator/=(const Vector2 &b);
	Vector2 operator/(const Vector2 &b) const;

	Vector2 &operator+=(float scalar);
	Vector2 operator+(float scalar) const;
	Vector2 &operator-=(float scalar);
	Vector2 operator-(float scalar) const;
	Vector2 &operator*=(float scalar);
	Vector2 operator*(float scalar) const;
	Vector2 &operator/=(float scalar);
	Vector2 operator/(float scalar) const;

	Vector2 operator-() const;

	bool operator==(const Vector2 &b) const;
	bool operator!=(const Vector2 &b) const;

	const float &operator[](int axis) const;
	float &operator[](int axis);

	operator String() const;
	operator Vector2i() const;

	enum Axis {
		AXIS_X = 0,
		AXIS_Y
	};

	union {
		struct {
			float x;
			float y;
		};

		float coordinates[2];
	};
};

#endif
