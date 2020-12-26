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

void ParallelSource::setDirection(vec3 dir) {
	direction = dir;
}

//==========


//===PointSource===

PointSource::PointSource(CString name, vec3 position, Color color) {
	this->name = name;
	this->position = position;
	this->color = color;
}

vec3 PointSource::getPosition() {
	return position;
}

void PointSource::setPosition(vec3 pos) {
	position = pos;
}

vec3 PointSource::getDirectionToPoint(Vertex to) {
	return position - to;
}

//==========