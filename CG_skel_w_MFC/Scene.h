	#pragma once

#include "gl/glew.h"
#include <vector>
#include <string>
#include "Renderer.h"
#include "Color.h"
#include "Camera.h"


using namespace std;

class Model {
protected:
	virtual ~Model() {}
	//void virtual draw()=0;
};


class Light {

};

class Scene {

	vector<Model*> models;
	vector<Light*> lights;
	vector<Camera*> cameras;
	Renderer *m_renderer;

public:
	Scene() {};
	Scene(Renderer *renderer);
	void loadOBJModel(string fileName);
	void draw();
	void drawDemo();
	void scale(double scale_factor);
	void rotateX(double theta);
	void rotateY(double theta);
	void rotateZ(double theta);
	void translate(vec4 vec);
	void toggleVertices();
	void toggleEdges();
	void toggleBB();
	void toggleVertexNormals();
	void toggleFaceNormals();
	
	void addCamera(Camera* camera);


	int activeModel;
	int activeLight;
	int activeCamera;
};