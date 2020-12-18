#include "Face.h"

Face::Face()
{
	for (int i = 0; i < 3; i++) {
		vertices[i] = nullptr;
		normals[i] = nullptr;
		textures[i] = nullptr;
	}
}

void Face::calcNormal() {
	vec4 v0 = *vertices[1] - *vertices[0];
	vec4 v1 = *vertices[2] - *vertices[1];
	center_normal = cross(v0, v1);
}

void Face::calcCenter() {
	center = (*vertices[0] + *vertices[1] + *vertices[2]) / 3;
}