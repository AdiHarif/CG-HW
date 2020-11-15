#pragma once
#include "scene.h"
#include "vec.h"
#include "mat.h"
#include <string>

#include "Renderer.h"

using namespace std;

class MeshModel : public Model
{
protected :
	MeshModel() {}

	//vector<FaceIdcs> faces;
	vector<vec3> vertices;
	vector<vec3> vertex_positions;
	//add more attributes
	vec4 position;
	mat4 _world_transform;
	mat3 _normal_transform;


public:

	MeshModel(string fileName);
	~MeshModel(void);
	void loadFile(string fileName);
	void draw(Renderer* renderer);

	void Translate(vec3);
	void Scale(vec3);
	void RotateX(GLfloat);
	void RotateY(GLfloat);
	void RotateZ(GLfloat);
};
