#ifndef RECT2I_H
#define RECT2I_H

#include "vector2i.h"

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

#endif