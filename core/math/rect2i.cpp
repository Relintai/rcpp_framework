#include "rect2i.h"

#include "math.h"

float Rect2i::get_area() const {
	return w * h;
}

bool Rect2i::intersects(const Rect2i &b) const {
	if (x >= (b.x + b.w))
		return false;

	if ((x + w) <= b.x)
		return false;

	if (y >= (b.y + b.h))
		return false;

	if ((y + h) <= b.y)
		return false;

	return true;
}

bool Rect2i::intersects_include_borders(const Rect2i &b) const {
	if (x > (b.x + b.w))
		return false;

	if ((x + w) < b.x)
		return false;

	if (y > (b.y + b.h))
		return false;

	if ((y + h) < b.y)
		return false;

	return true;
}

bool Rect2i::encloses(const Rect2i &b) const {
	return (b.x >= x) && (b.y >= y) &&
		   ((b.x + b.w) <= (x + w)) &&
		   ((b.y + b.h) <= (y + h));
}

bool Rect2i::has_no_area() const {
	if (w == 0 && h == 0) {
		return true;
	}

	return false;
}

bool Rect2i::has_point(const int px, const int py) const {
	if (px > x && px < x + w && py > y && py < py + h) {
		return true;
	}

	return false;
}

bool Rect2i::is_equal_approx(const Rect2i &b) const {
	if (x + EPSILON < b.x && x - EPSILON > b.x && y + EPSILON < b.y && y - EPSILON > b.y &&
			w + EPSILON < b.w && w - EPSILON > b.w && h + EPSILON < b.h && h - EPSILON > b.h) {
		return true;
	}

	return false;
}

void Rect2i::grow(const int by) {
	x -= by;
	y -= by;
	h += by;
	w += by;
}

void Rect2i::shrink(const int by) {
	x += by;
	y += by;
	h -= by;
	w -= by;
}

//Taken from the Godot Engine (MIT License)
//Copyright (c) 2007-2021 Juan Linietsky, Ariel Manzur.
//Copyright (c) 2014-2021 Godot Engine contributors (cf. AUTHORS.md).
void Rect2i::expand_to(const Vector2i &p_vector) {
	Vector2i begin = Vector2i(x, y);
	Vector2i end = Vector2i(x, y) + Vector2i(w, h);

	if (p_vector.x < begin.x) {
		begin.x = p_vector.x;
	}
	if (p_vector.y < begin.y) {
		begin.y = p_vector.y;
	}

	if (p_vector.x > end.x) {
		end.x = p_vector.x;
	}
	if (p_vector.y > end.y) {
		end.y = p_vector.y;
	}

	x = begin.x;
	y = begin.y;
	w = end.x - begin.x;
	h = end.y - begin.y;
}

Rect2i &Rect2i::operator+=(const Rect2i &b) {
	x += b.x;
	y += b.y;
	w += b.w;
	h += b.h;

	return *this;
}

Rect2i &Rect2i::operator-=(const Rect2i &b) {
	x -= b.x;
	y -= b.y;
	w -= b.w;
	h -= b.h;

	return *this;
}

Rect2i operator+(Rect2i lhs, const Rect2i &rhs) {
	lhs.x += rhs.x;
	lhs.y += rhs.y;
	lhs.w += rhs.w;
	lhs.h += rhs.h;

	return lhs;
}

Rect2i operator-(Rect2i lhs, const Rect2i &rhs) {
	lhs.x -= rhs.x;
	lhs.y -= rhs.y;
	lhs.w -= rhs.w;
	lhs.h -= rhs.h;

	return lhs;
}

bool operator==(const Rect2i &a, const Rect2i &b) {
	return a.is_equal_approx(b);
}

bool operator!=(const Rect2i &a, const Rect2i &b) {
	return !a.is_equal_approx(b);
}

Rect2i::Rect2i() {
	x = 0;
	y = 0;
	w = 0;
	h = 0;
}

Rect2i::Rect2i(const Rect2i &b) {
	x = b.x;
	y = b.y;
	w = b.w;
	h = b.h;
}

Rect2i::Rect2i(const int rx, const int ry) {
	x = rx;
	y = ry;
	w = 0;
	h = 0;
}

Rect2i::Rect2i(const int rx, const int ry, const int rw, const int rh) {
	x = rx;
	y = ry;
	w = rw;
	h = rh;
}

Rect2i::Rect2i(const Vector2i &position, const Vector2i &size) {
	x = position.x;
	y = position.y;
	w = size.x;
	h = size.y;
}
