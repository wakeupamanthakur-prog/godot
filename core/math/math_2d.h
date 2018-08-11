/*************************************************************************/
/*  math_2d.h                                                            */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           GODOT ENGINE                                */
/*                      https://godotengine.org                          */
/*************************************************************************/
/* Copyright (c) 2007-2018 Juan Linietsky, Ariel Manzur.                 */
/* Copyright (c) 2014-2018 Godot Engine contributors (cf. AUTHORS.md)    */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

#ifndef MATH_2D_H
#define MATH_2D_H

#include "transform_2d.h"
/**
	@author Juan Linietsky <reduzio@gmail.com>
*/
/* INTEGER STUFF */

struct Point2i {

	union {
		int x;
		int width;
	};
	union {
		int y;
		int height;
	};

	_FORCE_INLINE_ int &operator[](int p_idx) {
		return p_idx ? y : x;
	}
	_FORCE_INLINE_ const int &operator[](int p_idx) const {
		return p_idx ? y : x;
	}

	Point2i operator+(const Point2i &p_v) const;
	void operator+=(const Point2i &p_v);
	Point2i operator-(const Point2i &p_v) const;
	void operator-=(const Point2i &p_v);
	Point2i operator*(const Point2i &p_v1) const;

	Point2i operator*(const int &rvalue) const;
	void operator*=(const int &rvalue);

	Point2i operator/(const Point2i &p_v1) const;

	Point2i operator/(const int &rvalue) const;

	void operator/=(const int &rvalue);

	Point2i operator-() const;
	bool operator<(const Point2i &p_vec2) const { return (x == p_vec2.x) ? (y < p_vec2.y) : (x < p_vec2.x); }
	bool operator>(const Point2i &p_vec2) const { return (x == p_vec2.x) ? (y > p_vec2.y) : (x > p_vec2.x); }

	bool operator==(const Point2i &p_vec2) const;
	bool operator!=(const Point2i &p_vec2) const;

	real_t get_aspect() const { return width / (real_t)height; }

	operator String() const { return String::num(x) + ", " + String::num(y); }

	operator Vector2() const { return Vector2(x, y); }
	inline Point2i(const Vector2 &p_vec2) {
		x = (int)p_vec2.x;
		y = (int)p_vec2.y;
	}
	inline Point2i(int p_x, int p_y) {
		x = p_x;
		y = p_y;
	}
	inline Point2i() {
		x = 0;
		y = 0;
	}
};

typedef Point2i Size2i;

struct Rect2i {

	Point2i position;
	Size2i size;

	const Point2i &get_position() const { return position; }
	void set_position(const Point2i &p_pos) { position = p_pos; }
	const Point2i &get_size() const { return size; }
	void set_size(const Point2i &p_size) { size = p_size; }

	int get_area() const { return size.width * size.height; }

	inline bool intersects(const Rect2i &p_rect) const {
		if (position.x > (p_rect.position.x + p_rect.size.width))
			return false;
		if ((position.x + size.width) < p_rect.position.x)
			return false;
		if (position.y > (p_rect.position.y + p_rect.size.height))
			return false;
		if ((position.y + size.height) < p_rect.position.y)
			return false;

		return true;
	}

	inline bool encloses(const Rect2i &p_rect) const {

		return (p_rect.position.x >= position.x) && (p_rect.position.y >= position.y) &&
			   ((p_rect.position.x + p_rect.size.x) < (position.x + size.x)) &&
			   ((p_rect.position.y + p_rect.size.y) < (position.y + size.y));
	}

	inline bool has_no_area() const {

		return (size.x <= 0 || size.y <= 0);
	}
	inline Rect2i clip(const Rect2i &p_rect) const { /// return a clipped rect

		Rect2i new_rect = p_rect;

		if (!intersects(new_rect))
			return Rect2i();

		new_rect.position.x = MAX(p_rect.position.x, position.x);
		new_rect.position.y = MAX(p_rect.position.y, position.y);

		Point2 p_rect_end = p_rect.position + p_rect.size;
		Point2 end = position + size;

		new_rect.size.x = (int)(MIN(p_rect_end.x, end.x) - new_rect.position.x);
		new_rect.size.y = (int)(MIN(p_rect_end.y, end.y) - new_rect.position.y);

		return new_rect;
	}

	inline Rect2i merge(const Rect2i &p_rect) const { ///< return a merged rect

		Rect2i new_rect;

		new_rect.position.x = MIN(p_rect.position.x, position.x);
		new_rect.position.y = MIN(p_rect.position.y, position.y);

		new_rect.size.x = MAX(p_rect.position.x + p_rect.size.x, position.x + size.x);
		new_rect.size.y = MAX(p_rect.position.y + p_rect.size.y, position.y + size.y);

		new_rect.size = new_rect.size - new_rect.position; //make relative again

		return new_rect;
	};
	bool has_point(const Point2 &p_point) const {
		if (p_point.x < position.x)
			return false;
		if (p_point.y < position.y)
			return false;

		if (p_point.x >= (position.x + size.x))
			return false;
		if (p_point.y >= (position.y + size.y))
			return false;

		return true;
	}

	bool no_area() { return (size.width <= 0 || size.height <= 0); }

	bool operator==(const Rect2i &p_rect) const { return position == p_rect.position && size == p_rect.size; }
	bool operator!=(const Rect2i &p_rect) const { return position != p_rect.position || size != p_rect.size; }

	Rect2i grow(int p_by) const {

		Rect2i g = *this;
		g.position.x -= p_by;
		g.position.y -= p_by;
		g.size.width += p_by * 2;
		g.size.height += p_by * 2;
		return g;
	}

	inline void expand_to(const Point2i &p_vector) {

		Point2i begin = position;
		Point2i end = position + size;

		if (p_vector.x < begin.x)
			begin.x = p_vector.x;
		if (p_vector.y < begin.y)
			begin.y = p_vector.y;

		if (p_vector.x > end.x)
			end.x = p_vector.x;
		if (p_vector.y > end.y)
			end.y = p_vector.y;

		position = begin;
		size = end - begin;
	}

	operator String() const { return String(position) + ", " + String(size); }

	operator Rect2() const { return Rect2(position, size); }
	Rect2i(const Rect2 &p_r2) :
			position(p_r2.position),
			size(p_r2.size) {
	}
	Rect2i() {}
	Rect2i(int p_x, int p_y, int p_width, int p_height) :
			position(Point2(p_x, p_y)),
			size(Size2(p_width, p_height)) {
	}
	Rect2i(const Point2 &p_pos, const Size2 &p_size) :
			position(p_pos),
			size(p_size) {
	}
};

#endif
