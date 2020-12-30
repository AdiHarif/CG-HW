#pragma once
#include "vec.h"
#include "mat.h"
#include "Color.h"

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

	Color ambient_color;
	Color diffuse_color;
	Color specular_color;
	Color emit_color;

public:
	Face();
	Face(vec3 v, int normal_index = 0);
	Face(vec3 v, vec3 vn);
	Face(std::istream&, int normal_index = 0);

	void calcCenter(Vertex v0, Vertex v1, Vertex v2);

	Color getAmbientColor() { return ambient_color; }
	void setAmbientColor(Color c) { ambient_color = c; }

	Color getDiffuseColor() { return diffuse_color; }
	void setDiffuseColor(Color c) { diffuse_color = c; }

	Color getSpecularColor() { return specular_color; }
	void setSpecularColor(Color c) { specular_color = c; }

	Color getEmitColor() { return emit_color; }
	void setEmitColor(Color c) { emit_color = c; }


	friend class Renderer;
	friend class MeshModel;
	friend class PrimMeshModel;
	friend class NonUniformMeshModel;
};