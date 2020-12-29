#pragma once

#include "MeshModel.h"

class PrimMeshModel : public MeshModel {

	int type; // 0 == cube

public:
	PrimMeshModel() {
		vertices.push_back(vec4(1.0, -1.0, -1.0));
		vertices.push_back(vec4(1.0, -1.0, 1.0));
		vertices.push_back(vec4(-1.0, -1.0, 1.0));
		vertices.push_back(vec4(-1.0, -1.0, -1.0));
		vertices.push_back(vec4(1.0, 1.0, -0.999999));
		vertices.push_back(vec4(0.999999, 1.0, 1.000001));
		vertices.push_back(vec4(-1.0, 1.0, 1.0));
		vertices.push_back(vec4(-1.0, 1.0, -1.0));

		faces.push_back(Face(vec3(2, 3, 4)));
		faces.push_back(Face(vec3(8, 7, 6)));
		faces.push_back(Face(vec3(5, 6, 2)));
		faces.push_back(Face(vec3(6, 7, 3)));
		faces.push_back(Face(vec3(3, 7, 8)));
		faces.push_back(Face(vec3(1, 4, 8)));
		faces.push_back(Face(vec3(1, 2, 4)));
		faces.push_back(Face(vec3(5, 8, 6)));
		faces.push_back(Face(vec3(1, 5, 2)));
		faces.push_back(Face(vec3(2, 6, 3)));
		faces.push_back(Face(vec3(4, 3, 8)));
		faces.push_back(Face(vec3(5, 1, 8)));

		for (int i = 0; i < faces.size(); i++) {
			vec4 v0 = vertices[faces[i].vertices[1] - 1] - vertices[faces[i].vertices[0] - 1];
			vec4 v1 = vertices[faces[i].vertices[2] - 1] - vertices[faces[i].vertices[1] - 1];
			Normal n = cross(v0, v1);
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