#ifndef RECT2_H
#define RECT2_H

#include "vector2.h"
#include "math.h"

class Rect2i;

class Rect2 {
public:
	float get_area() const;
	bool intersects(const Rect2 &b) const;
	bool intersects_include_borders(const Rect2 &b) const;
	bool encloses(const Rect2 &b) const;
	bool has_no_area() const;
	bool has_point(const float px, const float py) const;
	bool is_equal_approx(const Rect2 &b) const;

	void grow(const float by);
	void shrink(const float by);

	void expand_to(const Vector2 &p_vector);
	inline Rect2 clip(const Rect2 &p_rect) const;

	inline Rect2 abs() const;

	Vector2 position() const;
	Vector2 size() const;

	Rect2 &operator+=(const Rect2 &b);
	Rect2 &operator-=(const Rect2 &b);

	friend Rect2 operator+(Rect2 lhs, const Rect2 &rhs);
	friend Rect2 operator-(Rect2 lhs, const Rect2 &rhs);

	friend bool operator==(const Rect2 &a, const Rect2 &b);
	friend bool operator!=(const Rect2 &a, const Rect2 &b);

	operator Rect2i() const;

	Rect2();
	Rect2(const Rect2 &b);
	Rect2(const float rx, const float ry);
	Rect2(const float rx, const float ry, const float rw, const float rh);
	Rect2(const Vector2 &position, const Vector2 &size);

	float x;
	float y;
	float w;
	float h;
};

// Taken from the Godot Engine (MIT License)
// Copyright (c) 2007-2021 Juan Linietsky, Ariel Manzur.
// Copyright (c) 2014-2021 Godot Engine contributors (cf. AUTHORS.md).
Rect2 Rect2::clip(const Rect2 &p_rect) const { /// return a clipped rect
	Rect2 new_rect = p_rect;

	if (!intersects(new_rect)) {
		return Rect2();
	}

	new_rect.x = MAX(p_rect.x, x);
	new_rect.y = MAX(p_rect.y, y);

	Vector2 p_rect_end = p_rect.position() + p_rect.size();
	Vector2 end = position() + size();

	new_rect.w = MIN(p_rect_end.x, end.x) - new_rect.x;
	new_rect.h = MIN(p_rect_end.y, end.y) - new_rect.y;

	return new_rect;
}

Rect2 Rect2::abs() const {
	return Rect2(x + MIN(w, 0), y + MIN(h, 0), ABS(w), ABS(h));
}

#endif