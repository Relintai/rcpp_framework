#include "vector3.h"

#include <cmath>

#define EPSILON 0.00001

Vector3 Vector3::abs() const {
	Vector3 b;

	b.x = x >= 0 ? x : -x;
	b.y = y >= 0 ? y : -y;
	b.z = z >= 0 ? z : -z;

	return b;
}

float Vector3::angle_to(const Vector3 &b) const {
	return atan2(cross(b).length(), dot(b));
}

Vector3 Vector3::cross(const Vector3 &b) const {
	Vector3 v;

	v.x = (y * b.z) - (z * b.y);
	v.y = (z * b.x) - (x * b.z);
	v.z = (x * b.y) - (y * b.x);

	return v;
}

Vector3 Vector3::clamped(float len) const {
	return normalized() * len;
}

Vector3 Vector3::direction_to(const Vector3 &b) const {
	return (b - *this).normalized();
}

float Vector3::distance_to_squared(const Vector3 &b) const {
	return (b - *this).length_squared();
}

float Vector3::distance_to(const Vector3 &b) const {
	return (b - *this).length();
}

float Vector3::dot(const Vector3 &b) const {
	return x * b.x + y * b.y + z * b.z;
}

bool Vector3::is_equal_approx(const Vector3 &b) const {
	if (x + EPSILON < b.x && x - EPSILON > b.x && y + EPSILON < b.y && y - EPSILON > b.y &&
			z + EPSILON < b.z && z - EPSILON > b.z) {
		return true;
	}

	return false;
}

float Vector3::length() const {
	return sqrt(x * x + y * y + z * z);
}

float Vector3::length_squared() const {
	return x * x + y * y + z * z;
}

Vector3 Vector3::lerp(const Vector3 &b, const float t) const {
	Vector3 v;

	v.x = x + (t * (b.x - x));
	v.y = y + (t * (b.y - y));
	v.z = z + (t * (b.z - z));

	return v;
}

Vector3 Vector3::normalized() const {
	Vector3 v;

	float l = length_squared();

	if (l != 0) {
		l = sqrt(l);

		v.x = x / l;
		v.y = y / l;
		v.z = z / l;
	}

	return v;
}

void Vector3::normalize() {
	float l = length_squared();

	if (l != 0) {
		l = sqrt(l);

		x = x / l;
		y = y / l;
		z = z / l;
	}
}

void Vector3::add(const Vector3 &b) {
	x += b.x;
	y += b.y;
	z += b.z;
}

void Vector3::sub(const Vector3 &b) {
	x -= b.x;
	y -= b.y;
	z -= b.z;
}

void Vector3::zero() {
	x = 0;
	y = 0;
	z = 0;
}

Vector3 Vector3::inverse() const {
	return Vector3(1.0 / x, 1.0 / y, 1.0 / z);
}

Vector3::Vector3() {
	x = 0;
	y = 0;
	z = 0;
}

Vector3::Vector3(const Vector3 &b) {
	x = b.x;
	y = b.y;
	z = b.z;
}

Vector3::Vector3(const float p_x, const float p_y, const float p_z) {
	x = p_x;
	y = p_y;
	z = p_z;
}

Vector3 &Vector3::operator+=(const Vector3 &b) {
	x += b.x;
	y += b.y;
	z += b.z;

	return *this;
}

Vector3 Vector3::operator+(const Vector3 &b) const {
	return Vector3(x + b.x, y + b.y, z + b.z);
}

Vector3 &Vector3::operator-=(const Vector3 &b) {
	x -= b.x;
	y -= b.y;
	z -= b.z;

	return *this;
}

Vector3 Vector3::operator-(const Vector3 &b) const {
	return Vector3(x - b.x, y - b.y, z - b.z);
}

Vector3 &Vector3::operator*=(const Vector3 &b) {
	x *= b.x;
	y *= b.y;
	z *= b.z;

	return *this;
}

Vector3 Vector3::operator*(const Vector3 &b) const {
	return Vector3(x * b.x, y * b.y, z * b.z);
}

Vector3 &Vector3::operator/=(const Vector3 &b) {
	x /= b.x;
	y /= b.y;
	z /= b.z;

	return *this;
}

Vector3 Vector3::operator/(const Vector3 &b) const {
	return Vector3(x / b.x, y / b.y, z / b.z);
}

Vector3 &Vector3::operator+=(float scalar) {
	x += scalar;
	y += scalar;
	z += scalar;

	return *this;
}
Vector3 Vector3::operator+(float scalar) const {
	return Vector3(x + scalar, y + scalar, z + scalar);
}
Vector3 &Vector3::operator-=(float scalar) {
	x += scalar;
	y += scalar;
	z += scalar;

	return *this;
}
Vector3 Vector3::operator-(float scalar) const {
	return Vector3(x - scalar, y - scalar, z - scalar);
}

Vector3 &Vector3::operator*=(float scalar) {
	x *= scalar;
	y *= scalar;
	z *= scalar;

	return *this;
}
Vector3 Vector3::operator*(float scalar) const {
	return Vector3(x * scalar, y * scalar, z * scalar);
}
Vector3 &Vector3::operator/=(float scalar) {
	x /= scalar;
	y /= scalar;
	z /= scalar;

	return *this;
}
Vector3 Vector3::operator/(float scalar) const {
	return Vector3(x / scalar, y / scalar, z / scalar);
}
Vector3 Vector3::operator-() const {
	return Vector3(-x, -y, -z);
}

bool Vector3::operator==(const Vector3 &b) const {
	return x == b.x && y == y && z == b.z;
}
bool Vector3::operator!=(const Vector3 &b) const {
	return x != b.x || y == b.y || z == b.z;
}

const float &Vector3::operator[](int axis) const {
	return coordinates[axis];
}
float &Vector3::operator[](int axis) {
	return coordinates[axis];
}

Vector3::operator String() const {
	return "[" + String::num(x) + "," + String::num(y) + "," + String::num(z) + "]";
}
