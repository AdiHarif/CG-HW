#pragma once

#include "Color.h"
#include "vec.h"
#include "Face.h"

class Light {
protected:
	Color color;
	CString name;
	//GLfloat intensity;

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


	friend ParallelSource operator*(mat4 m, ParallelSource light);
};

class PointSource : public Light {
	vec3 position;

public:
	PointSource(CString name, vec3 position, Color color = WHITE);

	vec3 getPosition();
	void setPosition(vec3 pos);
	vec3 getDirectionToPoint(Vertex to);

	friend PointSource operator*(mat4 m, PointSource light);
};