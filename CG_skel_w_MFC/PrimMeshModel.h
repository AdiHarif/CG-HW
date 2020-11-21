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

		initBoundingBox(vec4(-1.0, -1.0, -1.0), vec4(1.0, 1.0, 1.0));

		computeFacesNormals();
		position = vec4(0.0, 0.0, 0.0, 1.0);
		active_mesh_color = { 0.0, 1.0, 0.0 };
		inactive_mesh_color = { 0.4, 0.4, 0.4 };
		vertex_normals_color = { 1.0, 0.0, 1.0 };
		faces_normals_color = { 1.0, 0.0, 0.0 };
		bb_color = { 1.0, 1.0, 1.0 };
	}
};