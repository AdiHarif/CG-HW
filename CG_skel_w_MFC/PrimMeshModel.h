#pragma once

#include "MeshModel.h"

class PrimMeshModel : public MeshModel {

	int type; // 0 == cube

public:

	PrimMeshModel(string file_name) : MeshModel(file_name) {}
};