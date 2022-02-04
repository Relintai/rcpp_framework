#ifndef RECT2I_H
#define RECT2I_H

#include "vector2i.h"
#include "math.h"

class Rect2i {
public:
	float get_area() const;
	bool intersects(const Rect2i &b) const;
	bool intersects_include_borders(const Rect2i &b) const;
	bool encloses(const Rect2i &b) const;
	bool has_no_area() const;
	bool has_point(const int px, const int py) const;
	bool is_equal_approx(const Rect2i &b) const;

	void grow(const int by);
	void shrink(const int by);

	void expand_to(const Vector2i &p_vector);
	inline Rect2i clip(const Rect2i &p_rect) const;

	Vector2i position() const;
	Vector2i size() const;

	Rect2i &operator+=(const Rect2i &b);
	Rect2i &operator-=(const Rect2i &b);

	friend Rect2i operator+(Rect2i lhs, const Rect2i &rhs);
	friend Rect2i operator-(Rect2i lhs, const Rect2i &rhs);

	friend bool operator==(const Rect2i &a, const Rect2i &b);
	friend bool operator!=(const Rect2i &a, const Rect2i &b);

	Rect2i();
	Rect2i(const Rect2i &b);
	Rect2i(const int rx, const int ry);
	Rect2i(const int rx, const int ry, const int rw, const int rh);
	Rect2i(const Vector2i &position, const Vector2i &size);

	int x;
	int y;
	int w;
	int h;
};

// Taken from the Godot Engine (MIT License)
// Copyright (c) 2007-2021 Juan Linietsky, Ariel Manzur.
// Copyright (c) 2014-2021 Godot Engine contributors (cf. AUTHORS.md).
inline Rect2i Rect2i::clip(const Rect2i &p_rect) const { /// return a clipped rect
	Rect2i new_rect = p_rect;

	if (!intersects(new_rect)) {
		return Rect2i();
	}

	new_rect.x = MAX(p_rect.x, x);
	new_rect.y = MAX(p_rect.y, y);

	Vector2i p_rect_end = p_rect.position() + p_rect.size();
	Vector2i end = position() + size();

	new_rect.w = MIN(p_rect_end.x, end.x) - new_rect.x;
	new_rect.h = MIN(p_rect_end.y, end.y) - new_rect.y;

	return new_rect;
}

#endif