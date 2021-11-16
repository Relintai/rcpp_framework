#ifndef VECTOR3_H
#define VECTOR3_H

#include "core/string.h"

class Vector3 {
public:
	Vector3 abs() const;
	float angle_to(const Vector3 &b) const;
	Vector3 cross(const Vector3 &b) const;
	Vector3 clamped(float len) const;
	Vector3 direction_to(const Vector3 &b) const;
	float distance_to_squared(const Vector3 &b) const;
	float distance_to(const Vector3 &b) const;
	float dot(const Vector3 &b) const;
	bool is_equal_approx(const Vector3 &b) const;

	float length() const;
	float length_squared() const;

	Vector3 lerp(const Vector3 &b, const float t) const;

	Vector3 normalized() const;
	void normalize();

	void add(const Vector3 &b);
	void sub(const Vector3 &b);

	void zero();
	Vector3 inverse() const;

	Vector3();
	Vector3(const Vector3 &b);
	Vector3(const float p_x, const float p_y, const float p_z);

	Vector3 &operator+=(const Vector3 &b);
	Vector3 operator+(const Vector3 &b) const;
	Vector3 &operator-=(const Vector3 &b);
	Vector3 operator-(const Vector3 &b) const;
	Vector3 &operator*=(const Vector3 &b);
	Vector3 operator*(const Vector3 &b) const;
	Vector3 &operator/=(const Vector3 &b);
	Vector3 operator/(const Vector3 &b) const;

	Vector3 &operator+=(float scalar);
	Vector3 operator+(float scalar) const;
	Vector3 &operator-=(float scalar);
	Vector3 operator-(float scalar) const;
	Vector3 &operator*=(float scalar);
	Vector3 operator*(float scalar) const;
	Vector3 &operator/=(float scalar);
	Vector3 operator/(float scalar) const;

	Vector3 operator-() const;

	bool operator==(const Vector3 &b) const;
	bool operator!=(const Vector3 &b) const;

	const float &operator[](int axis) const;
	float &operator[](int axis);

	operator String() const;

	enum Axis {
		AXIS_X = 0,
		AXIS_Y,
		AXIS_Z
	};

	union {
		struct {
			float x;
			float y;
			float z;
		};

		float coordinates[3];
	};
};

#endif
