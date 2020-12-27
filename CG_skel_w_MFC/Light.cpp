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

ParallelSource operator*(mat4 m, ParallelSource light) {

	vec4 dir = m * light.direction;
	dir /= dir.w;

	return ParallelSource(light.name, normalize(vec3(dir.x,dir.y,dir.z)) , light.color);
}


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


PointSource operator*(mat4 m, PointSource light) {

	vec4 dir = m * light.position;
	dir /= dir.w;

	return PointSource(light.name, vec3(dir.x, dir.y, dir.z), light.color);
}
