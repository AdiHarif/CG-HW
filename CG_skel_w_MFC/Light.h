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
	void setColor(Color c) { color = c; }
	
};

class ParallelSource : public Light {
	vec3 direction;

public:
	ParallelSource(CString name, vec3 direction, Color color = WHITE);
	
	vec3 getDirection();
	void setDirection(vec3 dir);
};

class PointSource : public Light {
	vec3 position;

public:
	PointSource(CString name, vec3 position, Color color = WHITE);

	vec3 getPosition();
	void setPosition(vec3 pos);
	vec3 directionToPoint(vec3 to);
};