#pragma once
#include "vec.h"
#include "StdAfx.h"

#define RED { 1.0, 0.0, 0.0 }
#define GREEN { 0.0, 1.0, 0.0 }
#define BLUE { 0.0, 0.0, 1.0 }
#define WHITE { 1.0, 1.0, 1.0 }
#define INACTIVE_GRAY { 0.4, 0.4, 0.4 }
#define PURPLE { 1.0, 0.0, 1.0 }
#define YELLOW { 1.0, 1.0, 0.0 }


struct Color{
	float r = 0.0;
	float g = 0.0;
	float b = 0.0;

	//===C'tors / Destructors===
	Color() { r = 1.0; g = 1.0; b = 1.0; }
	Color(float r, float g, float b) : r(r), g(g), b(b) {}
	Color(vec3 v) : r(v.x), g(v.x), b(v.x) {}
	Color(const Color& c) { r = c.r; g = c.g; b = c.b; }
	//==========

	//===Operators===
	Color operator + (const Color c) const {
		return { min(1, r + c.r), min(1, g + c.g), min(1, b + c.b) };
	}

	Color& operator += (const Color c) {
		r += c.r; g += c.g; b += c.b;
		return *this;
	}

	Color operator * (const Color c) const {
		return { r * c.r, g * c.g, b * c.b };
	}

	Color& operator *= (const Color c)
	{
		r *= c.r;  g *= c.g;  b *= c.b;  return *this;
	}

	bool operator ==(Color c) {
		return (r == c.r) && (g == c.g) && (b == c.b);
	}

	Color operator * (const GLfloat s) const
	{
		return { s * r, s * g, s * b };
	}
	//==========

	void floorToOne() {
		r = min(1, r); 
		g = min(1, g); 
		b = min(1, b);
	}
};
