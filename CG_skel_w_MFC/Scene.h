#pragma once

#include "gl/glew.h"
#include <vector>
#include <string>
//#include "Renderer.h"
#include "Color.h"
#include "Camera.h"
#include "Light.h"
#include "InitShader.h"
#include "GL\freeglut.h"
#include "Model.h"

#define ALL_MODELS_ACTIVE -1
#define NO_MODELS_ACTIVE -2

using namespace std;

class Scene {

	struct GLInfo {
		GLuint program;
		GLuint vao;
	} gl_info;

	
	vector<Model*> models;

	vector<ParallelSource> parallel_sources;
	vector<PointSource> point_sources;

	vector<Camera*> cameras;
	Renderer *m_renderer;

	int active_model;
	int active_light;
	int active_camera;
	Color ambient_light_color;
	//GLfloat ambient_light_intensity;

	bool f_draw_cameras = false;
	bool f_draw_lights = false;

	bool f_blur;
	bool f_bloom;
	float light_bloom_threshold;

	//mat4 tw;

	friend class Renderer;
public:
	//===C'tors / Destructors===
	Scene() {}; //unimplemented
	Scene(Renderer *renderer);
	~Scene();
	//==========

	//===Getters/Setters===
	Model* getActiveModel();
	Camera* getActiveCamera();
	//==========


	//===Drawing Functions===
	void draw();
	void drawModel(MeshModel* m);
	//void drawDemo();
	//==========

	//===Models Interface===
	void loadOBJModel(string fileName);
	void loadPrimModel();
	void loadNonUniform();
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
	void togglePolyMode();
	void toggleBB();
	void toggleVertexNormals();
	void toggleFaceNormals();
	void toggleCameras();
	void toggleLights();
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
	vector<ParallelSource>* getParallelSources() { return &parallel_sources; }
	vector<PointSource>* getPointSources() { return &point_sources; }
	Color getAmbientColor() { return ambient_light_color; }
	void setAmbientColor(Color c) { ambient_light_color = c; }
	//====
	//ParallelSource Interface
	void addParallelSource(ParallelSource parallel_source);
	//====
	
	//PointSource Interface
	void addPointSource(PointSource point_source);
	//====


	void toggleAntiAliasing();
	void toggleFog();
	void toggleBlur();
	void toggleBloom();

	void updateActiveModelFacesColors();
};