#pragma once

#include "gl/glew.h"
#include <vector>
#include <string>
#include "Color.h"
#include "Camera.h"
#include "Light.h"
#include "InitShader.h"
#include "GL\freeglut.h"
#include "Model.h"

#define ALL_MODELS_ACTIVE -1
#define NO_MODELS_ACTIVE -2

#define AMBIENT_METHODS_COUNT 4
#define SHADING_METHODS_COUNT 4
#define SPECIAL_PROGRAMS_COUNT 1

#define COLOR_ANIMATIONS_METHODS_COUNT 2
#define VERTEX_ANIMATIONS_METHODS_COUNT 1

using namespace std;

typedef enum {
	AMBIENT,
	TEXTURE,
	PLANE_TEXTURE,
	SPHERE_TEXTURE
} AmbientMethod;

typedef enum {
	FLAT_SHADING,
	GOURAUD_SHADING,
	PHONG_SHADING,
	TOON_SHADING
} ShadingMethod;

typedef enum {
	SILHOUETTE
} SpecialShaders;

typedef enum {
	SMOOTH,
	WAVE
} ColorAnimationMethod;

typedef enum {
	RANDOM_DIRECTION
} VertexAnimationMethod;

class Scene {
	AmbientMethod active_ambient_method;
	ShadingMethod active_shading_method;
	ColorAnimationMethod active_color_animation_method;
	VertexAnimationMethod active_vertex_animation_method;

	GLuint ambient_programs[AMBIENT_METHODS_COUNT];
	GLuint shading_programs[SHADING_METHODS_COUNT];
	GLuint special_programs[SPECIAL_PROGRAMS_COUNT];

	GLuint color_animation_programs[COLOR_ANIMATIONS_METHODS_COUNT];
	GLuint vertex_animation_programs[VERTEX_ANIMATIONS_METHODS_COUNT];


	vector<Model*> models;
	int active_model;

	vector<Camera*> cameras;
	int active_camera;

	Color ambient_light_color;
	vector<ParallelSource> parallel_sources;
	vector<PointSource> point_sources;

	bool f_draw_cameras = false;
	bool f_draw_lights = false;

	bool is_color_animation_active = false;
	bool is_vertex_animation_active = false;

public:
	//===C'tors / Destructors===
	Scene();
	~Scene();
	//==========

	//===Getters/Setters===
	Camera* getActiveCamera();
	Model* getActiveModel();
	//==========

	//===Drawing Functions===
	void draw();
	//==========

	//===Models Interface===
	void loadOBJModel(string fileName);
	void loadTexture(string fileName);
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
	void toggleShadingMethod();
	void toggleAmbientMethod();
	void toggleColorAnimationMethod();
	void toggleIsColorAnimationActive();
	void toggleIsVertexAnimationActive();

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
	vector<ParallelSource>* getParallelSources() { return &parallel_sources; }
	vector<PointSource>* getPointSources() { return &point_sources; }

	Color getAmbientColor() { return ambient_light_color; }
	void setAmbientColor(Color c) { ambient_light_color = c; }

	void addParallelSource(ParallelSource parallel_source);
	void addPointSource(PointSource point_source);
	//==========

	//===OpenGL===
	//void Scene::bindAttributesToProgram(MeshModel* model, GLuint program);

	void setupAmbientProgram(MeshModel* model);
	void setupShadingProgram(MeshModel* model, Light* l);
	void setupSpecialProgram(MeshModel* model, SpecialShaders shader);
	void setupColorAnimationProgram(MeshModel* m);
	void setupVertexAnimationProgram(MeshModel* m);
	void bindBufferToProgram(MeshModel* model, GLuint program, GLuint vbo, GLchar* variable_name, boolean is_normalized);
	//void bindAttributeToProgram(MeshModel* model, GLuint program, GLuint vbo, GLchar* variable_name, boolean is_normalized);

	//void bindAttributesToGouraudShader(MeshModel* model);
	//==========

	//===Animations===
	bool getIsColorAnimationActive();
	bool getIsVertexAnimationActive();
	ColorAnimationMethod getActiveColorAnimationMethod();

	void updateAllModelsHSVColor();
	void updateAllModelsWaveThreshold();
	//==========

};