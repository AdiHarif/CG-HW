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
		vertex_normals.push_back(Normal(0.00000, 0.70711, 0.70711));
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

		faces.push_back(Face(vec3(1, 13, 4), vec3(1, 2, 1)));
		faces.push_back(Face(vec3(4, 13, 3), vec3(3, 4, 3)));
		faces.push_back(Face(vec3(3, 13, 2), vec3(5, 6, 5)));
		faces.push_back(Face(vec3(2, 13, 1), vec3(7, 8, 7)));
		faces.push_back(Face(vec3(4, 9, 1), vec3(1, 9, 1)));
		faces.push_back(Face(vec3(8, 9, 4), vec3(10, 11, 10)));
		faces.push_back(Face(vec3(5, 9, 8), vec3(12, 13, 12)));
		faces.push_back(Face(vec3(1, 9, 5), vec3(14, 15, 14)));
		faces.push_back(Face(vec3(5, 14, 6), vec3(16, 17, 16)));
		faces.push_back(Face(vec3(8, 14, 5), vec3(12, 18, 12)));
		faces.push_back(Face(vec3(7, 14, 8), vec3(19, 20, 19)));
		faces.push_back(Face(vec3(6, 14, 7), vec3(21, 22, 21)));
		faces.push_back(Face(vec3(3, 11, 7), vec3(23, 24, 23)));
		faces.push_back(Face(vec3(2, 11, 3), vec3(5, 25, 5)));
		faces.push_back(Face(vec3(6, 11, 2), vec3(26, 27, 26)));
		faces.push_back(Face(vec3(7, 11, 6), vec3(21, 28, 21)));
		faces.push_back(Face(vec3(3, 12, 4), vec3(3, 29, 3)));
		faces.push_back(Face(vec3(4, 12, 8), vec3(10, 30, 10)));
		faces.push_back(Face(vec3(8, 12, 7), vec3(19, 12, 19)));
		faces.push_back(Face(vec3(7, 12, 3), vec3(23, 32, 23)));
		faces.push_back(Face(vec3(1, 10, 2), vec3(7, 33, 7)));
		faces.push_back(Face(vec3(2, 10, 6), vec3(26, 34, 26)));
		faces.push_back(Face(vec3(6, 10, 5), vec3(16, 35, 16)));
		faces.push_back(Face(vec3(5, 10, 1), vec3(14, 36, 14)));



		vec4 min = vec4(-0.6, -0.6, -2.6);
		vec4 max = vec4(2.6, 2.6, 0.6);
		initBoundingBox(min, max);

		//setting model position:
		//position = vec4(0.0);

		//centering the model to (0,0,0):
		vec4 middle_offset = vec4((min.x + max.x) / 2, (min.y + max.y) / 2, (min.z + max.z) / 2, 1);
		mat4 t = translateMat(-middle_offset);
		for (vector<vec4>::iterator i = vertices.begin(); i != vertices.end(); i++) {
			(*i) = t * (*i);
		}
		for (vector<Edge>::iterator i = bb_edges.begin(); i != bb_edges.end(); i++) {
			(*i) = t * (*i);
		}

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

		//position = vec4(0.0, 0.0, 0.0, 1.0);
	}
};