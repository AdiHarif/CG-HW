#pragma once

#include "MeshModel.h"

class NonUniformMeshModel : public MeshModel {

public:
	NonUniformMeshModel() {
		vertices.push_back(vec4(0.00000, 0.00000, 0.00000));
		vertices.push_back(vec4(2.00000, 0.00000, 0.00000));
		vertices.push_back(vec4(2.00000, 2.00000, 0.00000));
		vertices.push_back(vec4(0.00000, 2.00000, 0.00000));
		vertices.push_back(vec4(0.00000, 0.00000, -2.00000));
		vertices.push_back(vec4(2.00000, 0.00000, -2.00000));
		vertices.push_back(vec4(2.00000, 2.00000, -2.00000));
		vertices.push_back(vec4(0.00000, 2.00000, -2.00000));
		vertices.push_back(vec4(-0.60000, 1.00000, -1.00000));
		vertices.push_back(vec4(1.00000, -0.60000, -1.00000));
		vertices.push_back(vec4(2.60000, 1.00000, -1.00000));
		vertices.push_back(vec4(1.00000, 2.60000, -1.00000));
		vertices.push_back(vec4(1.00000, 1.00000, 0.60000));
		vertices.push_back(vec4(1.00000, 1.00000, -2.60000));

		vertex_normals.push_back(Normal(-0.70711, 0.00000, 0.70711));
		vertex_normals.push_back(Normal(-0.51450, 0.00000, 0.85749));
		vertex_normals.push_back(Normal(0.00000, 0.70711 0.70711));
		vertex_normals.push_back(Normal(0.00000, 0.51450, 0.85749));
		vertex_normals.push_back(Normal(0.70711, 0.00000, 0.70711));
		vertex_normals.push_back(Normal(0.51450, 0.00000, 0.85749));
		vertex_normals.push_back(Normal(0.00000, -0.70711, 0.70711));
		vertex_normals.push_back(Normal(0.00000, -0.51450, 0.85749));
		vertex_normals.push_back(Normal(-0.85749, 0.00000, 0.51450));
		vertex_normals.push_back(Normal(-0.70711, 0.70711, 0.00000));
		vertex_normals.push_back(Normal(-0.85749, 0.51450, 0.00000));
		vertex_normals.push_back(Normal(-0.70711, 0.00000, -0.70711));
		vertex_normals.push_back(Normal(-0.85749, 0.00000, -0.51450));
		vertex_normals.push_back(Normal(-0.70711, -0.70711, 0.00000));
		vertex_normals.push_back(Normal(-0.85749, -0.51450, 0.00000));
		vertex_normals.push_back(Normal(0.00000, -0.70711, -0.70711));
		vertex_normals.push_back(Normal(0.00000, -0.51450, -0.85749));
		vertex_normals.push_back(Normal(-0.51450, 0.00000, -0.85749));
		vertex_normals.push_back(Normal(0.00000, 0.70711, -0.70711));
		vertex_normals.push_back(Normal(0.00000, 0.51450, -0.85749));
		vertex_normals.push_back(Normal(0.70711, 0.00000, -0.70711));
		vertex_normals.push_back(Normal(0.51450, 0.00000, -0.85749));
		vertex_normals.push_back(Normal(0.70711, 0.70711, 0.00000));
		vertex_normals.push_back(Normal(0.85749, 0.51450, 0.00000));
		vertex_normals.push_back(Normal(0.85749, 0.00000, 0.51450));
		vertex_normals.push_back(Normal(0.70711, -0.70711, 0.00000));
		vertex_normals.push_back(Normal(0.85749, -0.51450, 0.00000));
		vertex_normals.push_back(Normal(0.85749, 0.00000, -0.51450));
		vertex_normals.push_back(Normal(0.00000, 0.85749, 0.51450));
		vertex_normals.push_back(Normal(-0.51450, 0.85749, 0.00000));
		vertex_normals.push_back(Normal(0.00000, 0.85749, -0.51450));
		vertex_normals.push_back(Normal(0.51450, 0.85749, 0.00000));
		vertex_normals.push_back(Normal(0.00000, -0.85749, 0.51450));
		vertex_normals.push_back(Normal(0.51450, -0.85749, 0.00000));
		vertex_normals.push_back(Normal(0.00000, -0.85749, -0.51450));
		vertex_normals.push_back(Normal(-0.51450, -0.85749, 0.00000));

		faces.push_back(Face(vec3(1, 13, 4), ));

		for (int i = 0; i < faces.size(); i++) {

			Face& f = faces[i];

			Vertex v0, v1, v2;
			v0 = vertices[f.vertices[0] - 1];
			v1 = vertices[f.vertices[1] - 1];
			v2 = vertices[f.vertices[2] - 1];

			f.calcCenter(v0, v1, v2);

			vec4 e0 = vertices[faces[i].vertices[1] - 1] - vertices[faces[i].vertices[0] - 1];
			vec4 e1 = vertices[faces[i].vertices[2] - 1] - vertices[faces[i].vertices[1] - 1];
			Normal n = cross(e0, e1);
			face_normals.push_back(n);
			faces[i].normal = i;

		}

		vec4 min = vec4(-1.0, -1.0, -1.0);
		vec4 max = vec4(1.0, 1.0, 1.0);
		initBoundingBox(min, max);

		//setting model position:
		//position = vec4(0.0);

		//centering the model to (0,0,0):
		vec4 middle_offset = vec4((min.x + max.x) / 2, (min.y + max.y) / 2, (min.z + max.z) / 2);
		mat4 t = translateMat(-middle_offset);
		for (vector<vec4>::iterator i = vertices.begin(); i != vertices.end(); i++) {
			(*i) = t * (*i);
		}
		for (vector<Edge>::iterator i = bb_edges.begin(); i != bb_edges.end(); i++) {
			(*i) = t * (*i);
		}

		//position = vec4(0.0, 0.0, 0.0, 1.0);
	}
};