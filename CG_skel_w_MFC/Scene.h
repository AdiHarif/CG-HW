#pragma once

#include "gl/glew.h"
#include <vector>
#include <string>
#include "Renderer.h"
#include "Color.h"
#include "Camera.h"
#include "Light.h"

#define ALL_MODELS_ACTIVE -2

using namespace std;

class Model {
protected:
	Color diffuse_color;
	Color specular_color;
	Color ambient_color = WHITE;
	Color emit_color;

	GLfloat specular_reflection;
	GLfloat	diffuse_reflection;
	//GLfloat ambient_reflection = 1;
	GLfloat shininess;

	virtual ~Model() {}
	//void virtual draw()=0;
};


class Scene {

	vector<Model*> models;
	//vector<Light*> lights;
	vector<ParallelSource*> parallel_sources;
	vector<PointSource*> point_sources;
	vector<Camera*> cameras;
	Renderer *m_renderer;

	int active_model;
	int active_light;
	int active_camera;
	Color ambient_light_color;
	//GLfloat ambient_light_intensity;

	bool f_draw_cameras = false;

	mat4 tw;

public:
	//===C'tors / Destructors===
	Scene() {}; //unimplemented
	Scene(Renderer *renderer);
	~Scene();
	//==========

	//===Getters/Setters===
	//vector<Model*> getModels();
	//int getActiveModelIndex();
	//Model* getActiveModel();
	//int getActiveCameraIndex();
	Camera* getActiveCamera();
	//==========


	//===Drawing Functions===
	void draw();
	//void drawDemo();
	//==========

	//===Models Interface===
	void loadOBJModel(string fileName);
	void loadPrimModel();
	//void removeModel(int model);
	void removeSelection();

	void scaleSelection(double scale_factor);
	void scaleSelection(vec3 scale_by);
	void rotateSelectionX(double theta);
	void rotateSelectionY(double theta);
	void rotateSelectionZ(double theta);
	void translateSelection(vec4 vec);

	void activateNextModel();
	void activateLastModel();
	void changeAllModelsActiveness(bool is_active);
	//==========

	//===Display Toggles===
	/*void toggleVertices();
	void toggleEdges();*/
	void togglePolyMode();
	void toggleBB();
	void toggleVertexNormals();
	void toggleFaceNormals();
	void toggleCameras();
	//==========

	//===Cameras Interface===
	void addCamera(Camera* camera);
	void activateNextCamera();
	void activatePrevCamera();
	void activateLastCamera();
	void activeCameraToOrtho(const float left, const float right,
		const float bottom, const float top,
		const float z_near, const float z_far);
	void activeCameraToFrustum(const float left, const float right,
		const float bottom, const float top,
		const float z_near, const float z_far);
	void activeCameraToPerspective(const float fovy, const float aspect,
			const float z_near, const float z_far);
	void lookAtActiveModel();
	//void deactivateAllCameras();

	void toggleCameraProjection();

	void rotateCameraXAroundAt(float theta);
	void rotateCameraYAroundAt(float theta);
	void rotateCameraZAroundAt(float theta);

	void translateCameraC(vec4 v);
	void translateCameraWorld(vec4 v);
	void rotateCameraXWorld(float theta);
	void rotateCameraYWorld(float theta);
	void rotateCameraZWorld(float theta);

	void zoom(float amount);
	//==========

	//===Lights Interface===
	//General

	//====
	//ParallelSource Interface
	void addParallelSource(ParallelSource* parallel_source);
	//====
	
	//PointSource Interface
	void addPointSource(PointSource* point_source);
	//====
	//==========

	//===Other===
	void party();
	//==========
};