#include "Face.h"


Edge operator*(mat4& m, Edge& e) {
	return Edge(m * e.start, m * e.end);
}

Face::Face()
{
	for (int i = 0; i < 3; i++) {
		vertices[i] = vertex_normals[i] = textures[i] = 0;
	}
	normal = 0;
}

Face::Face(vec3 v, int normal_index) {
	for (int i = 0; i < 3; i++) {
		vertices[i] = v[i];
	}
	normal = normal_index;
}


Face::Face(std::istream& s, int normal_index) {
	for (int i = 0; i < 3; i++) {
		vertices[i] = vertex_normals[i] = textures[i] = 0;
	}

	char c;
	for (int i = 0; i < 3; i++)
	{
		s >> std::ws >> vertices[i] >> std::ws;
		if (s.peek() != '/')
			continue;
		s >> c >> std::ws;
		if (s.peek() == '/')
		{
			s >> c >> std::ws >> vertex_normals[i];
			continue;
		}
		else
			s >> textures[i];
		if (s.peek() != '/')
			continue;
		s >> c >> vertex_normals[i];
	}

	normal = normal_index;
}


void Face::calcCenter(Vertex v0, Vertex v1, Vertex v2) {
	v0 = v0 / v0.w;
	v1 = v1 / v1.w;
	v2 = v2 / v2.w;

	center = (v0 + v1 + v2) / 3;
}