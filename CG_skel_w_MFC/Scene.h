	#pragma once

#include "gl/glew.h"
#include <vector>
#include <string>
#include "Renderer.h"
#include "Color.h"
#include "Camera.h"

#define ALL_MODELS_ACTIVE -2

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

	int active_model;
	int active_light;
	int active_camera;

	mat4 tw;

public:
	//===C'tors===
	Scene() {}; //unimplemented
	Scene(Renderer *renderer);
	//==========

	//===Getters/Setters===
	vector<Model*> getModels();
	int getActiveModelIndex();
	int getActiveCameraIndex();
	Camera* getActiveCamera();
	//==========


	//===Drawing Functions===
	void draw();
	void drawDemo();
	//==========

	//===Models Interface===
	void loadOBJModel(string fileName);
	void loadPrimModel(string file_name);
	void removeModel(int model);

	void scaleSelection(double scale_factor);
	void rotateSelectionX(double theta);
	void rotateSelectionY(double theta);
	void rotateSelectionZ(double theta);
	void translateSelection(vec4 vec);

	void activateNextModel();
	void activateLastModel();
	void changeAllModelsActiveness(bool is_active);
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
	void activateNextCamera();
	void activatePrevCamera();
	void activateLastCamera();
	//void deactivateAllCameras();

	void toggleCameraProjection();

	void rotateCameraXAroundAt(float theta);
	void rotateCameraYAroundAt(float theta);
	void rotateCameraZAroundAt(float theta);

	void translateCameraC(vec4 v);
	//==========

	//===Other===
	void party();
	//==========
};