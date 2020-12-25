#include "Light.h"

//===ParallelSource===

ParallelSource::ParallelSource(CString name, vec3 direction, Color color) {
	this->name = name;
	this->direction = direction;
	this->color = color;
}

vec3 ParallelSource::getDirection() {
	return direction;
}

//==========


//===PointSource===

PointSource::PointSource(CString name, vec3 position, Color color) {
	this->name = name;
	position = position;
	color = color;
}

vec3 PointSource::directionToPoint(vec3 to) {//TODO: implement
	return vec3(0, 0, 0);
}

//==========