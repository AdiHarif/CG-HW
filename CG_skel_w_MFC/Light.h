#pragma once

#include "Color.h"
#include "vec.h"

class Light {
protected:
	Color color;

	virtual ~Light() {}

public:
	Color getColor() { return color; }
	
};

class ParallelSource : public Light {
	vec3 direction;

public:
	ParallelSource(vec3 direction, Color color = WHITE);
	
	vec3 getDirection();
};

class PointSource : public Light {
	vec3 position;

public:
	PointSource(vec3 position, Color color = WHITE);

	vec3 directionToPoint(vec3 to);
};