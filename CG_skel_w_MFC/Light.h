#pragma once

#include "Color.h"
#include "vec.h"

class Light {
protected:
	Color color;
	CString name;

	virtual ~Light() {}

public:
	CString getName() { return name; }
	Color getColor() { return color; }
	
};

class ParallelSource : public Light {
	vec3 direction;

public:
	ParallelSource(CString name, vec3 direction, Color color = WHITE);
	
	vec3 getDirection();
};

class PointSource : public Light {
	vec3 position;

public:
	PointSource(CString name, vec3 position, Color color = WHITE);

	vec3 directionToPoint(vec3 to);
};