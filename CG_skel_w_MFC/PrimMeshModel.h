#pragma once

#include "MeshModel.h"

class PrimMeshModel : public MeshModel {

	int type; // 0 == cube

public:

	//PrimMeshModel(string file_name) : MeshModel(file_name) {}
	PrimMeshModel() {
		vertices.push_back(vec4(1.0, -1.0, -1.0));
		vertices.push_back(vec4(1.0, -1.0, 1.0));
		vertices.push_back(vec4(-1.0, -1.0, 1.0));
		vertices.push_back(vec4(-1.0, -1.0, -1.0));
		vertices.push_back(vec4(1.0, 1.0, -0.999999));
		vertices.push_back(vec4(0.999999, 1.0, 1.000001));
		vertices.push_back(vec4(-1.0, 1.0, 1.0));
		vertices.push_back(vec4(-1.0, 1.0, -1.0));

		faces.push_back(FaceIdcs(vec4(2, 3, 4)));
		faces.push_back(FaceIdcs(vec4(8, 7, 6)));
		faces.push_back(FaceIdcs(vec4(5, 6, 2)));
		faces.push_back(FaceIdcs(vec4(6, 7, 3)));
		faces.push_back(FaceIdcs(vec4(3, 7, 8)));
		faces.push_back(FaceIdcs(vec4(1, 4, 8)));
		faces.push_back(FaceIdcs(vec4(1, 2, 4)));
		faces.push_back(FaceIdcs(vec4(5, 8, 6)));
		faces.push_back(FaceIdcs(vec4(1, 5, 2)));
		faces.push_back(FaceIdcs(vec4(2, 6, 3)));
		faces.push_back(FaceIdcs(vec4(4, 3, 8)));
		faces.push_back(FaceIdcs(vec4(5, 1, 8)));

		vec4 min = vec4(-1.0, -1.0, -1.0);
		vec4 max = vec4(1.0, 1.0, 1.0);
		initBoundingBox(min, max);
		//setting model position:
		position = vec4(0.0);
		//centering the model to (0,0,0):
		vec4 middle_offset = vec4((min.x + max.x) / 2, (min.y + max.y) / 2, (min.z + max.z) / 2);
		mat4 t = translateMat(-middle_offset);
		for (vector<vec4>::iterator i = vertices.begin(); i != vertices.end(); i++) {
			(*i) = t * (*i);
		}
		for (vector<vec4>::iterator i = bounding_box_vertices.begin(); i != bounding_box_vertices.end(); i++) {
			(*i) = t * (*i);
		}

		for (vector<FaceIdcs>::iterator it = faces.begin(); it != faces.end(); ++it) {
			for (int i = 0; i < 3; i++)
			{
				vertex_positions.push_back(vec4(vertices[it->v[i] - 1]));
			}
		}

		computeFacesNormals();
		position = vec4(0.0, 0.0, 0.0, 1.0);
		active_mesh_color = { 0.0, 1.0, 0.0 };
		inactive_mesh_color = { 0.4, 0.4, 0.4 };
		vertex_normals_color = { 1.0, 0.0, 1.0 };
		faces_normals_color = { 1.0, 0.0, 0.0 };
		bb_color = { 1.0, 1.0, 1.0 };
	}
};