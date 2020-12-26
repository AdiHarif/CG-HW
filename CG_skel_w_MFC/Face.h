#pragma once
#include "vec.h"
#include "mat.h"

typedef vec4 Vertex;

typedef vec4 Normal;

struct Edge {
	Vertex start;
	Vertex end;

	Edge(Vertex start, Vertex end) : start(start), end(end) {}

	friend Edge operator*(mat4& m, Edge& e);
};


class Face {
private:
	int vertices[3];
	int vertex_normals[3];
	int textures[3];

	int normal;
	Vertex center;

public:
	Face();
	Face(vec3, int normal_index = 0);
	Face(std::istream&, int normal_index = 0);

	void calcCenter(Vertex v0, Vertex v1, Vertex v2);
	

	friend class Renderer;
	friend class MeshModel;
	friend class PrimMeshModel;
};