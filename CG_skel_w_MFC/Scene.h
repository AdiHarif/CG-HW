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

	int activeModel;
	int activeLight;
	int activeCamera;

public:
	//===C'tors===
	Scene() {}; //unimplemented
	Scene(Renderer *renderer);
	//==========

	//===Drawing Functions===
	void draw();
	void drawDemo();
	//==========

	//===Models Interface===
	void loadOBJModel(string fileName);

	void scaleSelection(double scale_factor);
	void rotateSelectionX(double theta);
	void rotateSelectionY(double theta);
	void rotateSelectionZ(double theta);
	void translateSelection(vec4 vec);

	Model getActiveModel();
	void activateNextModel();
	//==========

	//===Display Toggles===
	void toggleVertices();
	void toggleEdges();
	void toggleBB();
	void toggleVertexNormals();
	void toggleFaceNormals();
	//==========

	//===Cameras Interface===
	void addCamera(Camera* camera);
	//==========

};