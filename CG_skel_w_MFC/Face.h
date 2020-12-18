#pragma once
#include "vec.h"

typedef vec4 Vertex;

typedef vec4 Normal;

struct Edge {
	Vertex start;
	Vertex end;

	Edge(Vertex start, Vertex end) : start(start), end(end) {}
};


class Face {

private:

	Vertex* vertices[3];
	Normal* normals[3];
	void* textures[3];

	Vertex center;
	Normal center_normal;

public:

	Face();

	void calcNormal();

	void calcCenter();

	friend class Renderer;
	friend class MeshModel;
};