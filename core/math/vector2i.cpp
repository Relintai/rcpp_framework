#include "vector2i.h"

#include <cmath>
#include "vector2.h"

Vector2i Vector2i::abs() const {
	Vector2i b;

	b.x = x >= 0 ? x : -x;
	b.y = y >= 0 ? y : -y;

	return b;
}

float Vector2i::angle() const {
	return atan2(x, y);
}

float Vector2i::angle_to(const Vector2i &b) const {
	return atan2(cross(b), dot(b));
}

float Vector2i::cross(const Vector2i &b) const {
	return x * b.y - y * b.x;
}

float Vector2i::distance_to_squared(const Vector2i &b) const {
	return (x - b.x) * (x - b.x) + (y - b.y) * (y - b.y);
}

float Vector2i::distance_to(const Vector2i &b) const {
	return sqrt((x - b.x) * (x - b.x) + (y - b.y) * (y - b.y));
}

float Vector2i::dot(const Vector2i &b) const {
	return x * b.x + y * b.y;
}

float Vector2i::length() const {
	return sqrt(x * x + y * y);
}

float Vector2i::length_squared() const {
	return x * x + y * y;
}

Vector2i Vector2i::lerp(const Vector2i &b, const float t) const {
	Vector2i v;

	v.x = x + (t * (b.x - x));
	v.y = y + (t * (b.y - y));

	return v;
}

void Vector2i::add(const Vector2i &b) {
	x += b.x;
	y += b.y;
}

void Vector2i::sub(const Vector2i &b) {
	x -= b.x;
	y -= b.y;
}

Vector2i::Vector2i() {
	x = 0;
	y = 0;
}

Vector2i::Vector2i(const Vector2i &b) {
	x = b.x;
	y = b.y;
}

Vector2i::Vector2i(const int p_x, const int p_y) {
	x = p_x;
	y = p_y;
}

Vector2i &Vector2i::operator+=(const Vector2i &b) {
	x += b.x;
	y += b.y;

	return *this;
}

Vector2i Vector2i::operator+(const Vector2i &b) const {
	return Vector2i(x + b.x, y + b.y);
}

Vector2i &Vector2i::operator-=(const Vector2i &b) {
	x -= b.x;
	y -= b.y;

	return *this;
}

Vector2i Vector2i::operator-(const Vector2i &b) const {
	return Vector2i(x - b.x, y - b.y);
}

Vector2i &Vector2i::operator*=(const Vector2i &b) {
	x *= b.x;
	y *= b.y;

	return *this;
}

Vector2i Vector2i::operator*(const Vector2i &b) const {
	return Vector2i(x * b.x, y * b.y);
}

Vector2i &Vector2i::operator/=(const Vector2i &b) {
	x /= b.x;
	y /= b.y;

	return *this;
}

Vector2i Vector2i::operator/(const Vector2i &b) const {
	return Vector2i(x / b.x, y / b.y);
}

Vector2i &Vector2i::operator+=(int scalar) {
	x += scalar;
	y += scalar;

	return *this;
}
Vector2i Vector2i::operator+(int scalar) const {
	return Vector2i(x + scalar, y + scalar);
}
Vector2i &Vector2i::operator-=(int scalar) {
	x += scalar;
	y += scalar;

	return *this;
}
Vector2i Vector2i::operator-(int scalar) const {
	return Vector2i(x - scalar, y - scalar);
}

Vector2i &Vector2i::operator*=(int scalar) {
	x *= scalar;
	y *= scalar;

	return *this;
}
Vector2i Vector2i::operator*(int scalar) const {
	return Vector2i(x * scalar, y * scalar);
}
Vector2i &Vector2i::operator/=(int scalar) {
	x /= scalar;
	y /= scalar;

	return *this;
}
Vector2i Vector2i::operator/(int scalar) const {
	return Vector2i(x / scalar, y / scalar);
}
Vector2i Vector2i::operator-() const {
	return Vector2i(-x, -y);
}

bool Vector2i::operator==(const Vector2i &b) const {
	return x == b.x && y == b.y;
}
bool Vector2i::operator!=(const Vector2i &b) const {
	return x != b.x || y != b.y;
}

const int &Vector2i::operator[](int axis) const {
	return coordinates[axis];
}
int &Vector2i::operator[](int axis) {
	return coordinates[axis];
}

Vector2i::operator String() const {
	return "[" + String::num(x) + "," + String::num(y) + "]";
}

Vector2i::operator Vector2() const {
	return Vector2(x, y);
}
