#ifndef VECTOR2I_H
#define VECTOR2I_H

#include "core/string.h"

class Vector2;

class Vector2i {
public:
	Vector2i abs() const;
	float angle() const;
	float angle_to(const Vector2i &b) const;
	float cross(const Vector2i &b) const;
	float distance_to_squared(const Vector2i &b) const;
	float distance_to(const Vector2i &b) const;
	float dot(const Vector2i &b) const;

	float length() const;
	float length_squared() const;

	Vector2i lerp(const Vector2i &b, const float t) const;

	void add(const Vector2i &b);
	void sub(const Vector2i &b);

	Vector2i();
	Vector2i(const Vector2i &b);
	Vector2i(const int p_x, const int p_y);

	Vector2i &operator+=(const Vector2i &b);
	Vector2i operator+(const Vector2i &b) const;
	Vector2i &operator-=(const Vector2i &b);
	Vector2i operator-(const Vector2i &b) const;
	Vector2i &operator*=(const Vector2i &b);
	Vector2i operator*(const Vector2i &b) const;
	Vector2i &operator/=(const Vector2i &b);
	Vector2i operator/(const Vector2i &b) const;

	Vector2i &operator+=(int scalar);
	Vector2i operator+(int scalar) const;
	Vector2i &operator-=(int scalar);
	Vector2i operator-(int scalar) const;
	Vector2i &operator*=(int scalar);
	Vector2i operator*(int scalar) const;
	Vector2i &operator/=(int scalar);
	Vector2i operator/(int scalar) const;

	Vector2i operator-() const;

	bool operator==(const Vector2i &b) const;
	bool operator!=(const Vector2i &b) const;

	const int &operator[](int axis) const;
	int &operator[](int axis);

	operator String() const;
	operator Vector2() const;

	enum Axis {
		AXIS_X = 0,
		AXIS_Y
	};

	union {
		struct {
			int x;
			int y;
		};

		int coordinates[2];
	};
};

#endif
