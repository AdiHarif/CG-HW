#include "Light.h"

//===ParallelSource===

ParallelSource::ParallelSource(vec3 direction, Color color) {
	this->direction = direction;
	this->color = color;
}

vec3 ParallelSource::getDirection() {
	return direction;
}

//==========


//===PointSource===

PointSource::PointSource(vec3 position, Color color) {
	position = position;
	color = color;
}

vec3 PointSource::directionToPoint(vec3 to) {//TODO: implement
	return vec3(0, 0, 0);
}

//==========