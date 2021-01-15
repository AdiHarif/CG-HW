#include "stdafx.h"
#include "Scene.h"
#include "MeshModel.h"
#include "PrimMeshModel.h"
#include "NonUniformMeshModel.h"
#include <string>

using namespace std;

Color camera_plus_color = { 1.0, 1.0, 0.0 };


Camera* Scene::getActiveCamera() {
	return cameras[active_camera];
}
//==========

//===C'tors / Destructors===

Scene::Scene() {
	active_model = NO_MODELS_ACTIVE;

	active_camera = -1;
	Camera* def_cam = new Camera(vec4(0.0, 0.0, 10.0));
	addCamera(def_cam);

	ambient_light_color = { 0.1, 0.1, 0.1 };
	parallel_sources.push_back(ParallelSource("Point Light 0", vec3(0.0, 0.0, -1.0), { 0.3, 0.1, 0.1 }));
	point_sources.push_back(PointSource("Point Light 0", vec3(1.5, 1.5, 1.5), { 1, 1, 1 }));

	gl_info.program = InitShader("vshader.glsl", "fshader.glsl");

}

Scene::~Scene() {
	for (vector<Camera*>::iterator i = cameras.begin(); i != cameras.end(); i++) {
		Camera* c = dynamic_cast<Camera*> ((*i));
		delete c;
	}
}
//==========

//===Drawing Functions===

void Scene::draw(){

	GLuint loc = glGetAttribLocation(gl_info.program, "v_position");
	glEnableVertexAttribArray(loc);
	glVertexAttribPointer(loc, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glClearColor(0.2, 0.2, 0.2, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	for (vector<Model*>::iterator i = models.begin(); i != models.end(); i++) {
		MeshModel* m = dynamic_cast<MeshModel*> ((*i));
		drawModel(m);
	}

	glutSwapBuffers();
}

void Scene::drawModel(MeshModel* m) {

	mat4 vt = this->cameras[active_camera]->tp * this->cameras[active_camera]->tc * m->tw * m->tm;

	glBindVertexArray(m->vao);
	GLuint vt_loc = glGetUniformLocation(gl_info.program, "v_transform");
	glUniformMatrix4fv(vt_loc, 1, GL_TRUE, vt);
	switch (m->draw_pref.poly_mode) {
	case DrawPref::VERTICES_ONLY: {
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		break;
	}
	case DrawPref::EDGES_ONLY: {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		break;
	}
	case DrawPref::FILLED: {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	}
	}

	glDrawArrays(GL_TRIANGLES, 0, m->faces.size()*3);
	glFlush();
}


//==========


//===Models Interface===

void Scene::loadOBJModel(string fileName)
{
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glUseProgram(gl_info.program);

	MeshModel *model = new MeshModel(fileName, vao);
	models.push_back(model);
	activateLastModel();

	vec4* vertex_positions = (vec4*)alloca(model->faces.size() * 3 * sizeof(vec4));
	for (int i = 0; i < model->faces.size(); i++) {
		vertex_positions[3 * i] = model->vertices[model->faces[i].vertices[0] - 1];
		vertex_positions[(3 * i) + 1] = model->vertices[model->faces[i].vertices[1] - 1];
		vertex_positions[(3 * i) + 2] = model->vertices[model->faces[i].vertices[2] - 1];
	}

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, model->faces.size()*3*sizeof(vec4), vertex_positions, GL_STATIC_DRAW);
}

void Scene::loadPrimModel() {
	//PrimMeshModel* model = new PrimMeshModel(file_name);
	PrimMeshModel* model = new PrimMeshModel();
	models.push_back(model);
	activateLastModel();
}

void Scene::loadNonUniform() {
	NonUniformMeshModel* model = new NonUniformMeshModel();
	models.push_back(model);
	activateLastModel();
}

void Scene::removeSelection() {
	if (models.empty())	return;
	if (active_model == ALL_MODELS_ACTIVE) {
		models.clear();
		return;
	}
	models.erase(models.begin() + active_model);
	activateLastModel();
}

Model* Scene::getActiveModel() {
	if (active_model == NO_MODELS_ACTIVE || active_model == ALL_MODELS_ACTIVE) {
		return NULL;
	}
	return models.at(active_model);
}

void Scene::scaleSelection(double factor) {
	scaleSelection(vec3(factor));
}

void Scene::scaleSelection(vec3 v) {
	if (active_model == ALL_MODELS_ACTIVE) {
		for (int i = 0; i < this->models.size(); i++) {
			MeshModel* m = dynamic_cast<MeshModel*> (this->models[i]);
			m->scale(v);
		}
	}
	else {
		MeshModel* m = dynamic_cast<MeshModel*> (this->models[active_model]);
		m->scale(v);
	}
}

void Scene::rotateSelectionX(double theta) {
	if (active_model == ALL_MODELS_ACTIVE) {
		for (int i = 0; i < this->models.size(); i++) {
			MeshModel* m = dynamic_cast<MeshModel*> (this->models[i]);
			m->rotateX(theta);
		}
	}
	else {
		MeshModel* m = dynamic_cast<MeshModel*> (this->models[active_model]);
		m->rotateX(theta);
	}
}

void Scene::rotateSelectionY(double theta) {
	if (active_model == ALL_MODELS_ACTIVE) {
		for (int i = 0; i < this->models.size(); i++) {
			MeshModel* m = dynamic_cast<MeshModel*> (this->models[i]);
			m->rotateY(theta);
		}
	}
	else {
		MeshModel* m = dynamic_cast<MeshModel*> (this->models[active_model]);
		m->rotateY(theta);
	}
}

void Scene::rotateSelectionZ(double theta) {
	if (active_model == ALL_MODELS_ACTIVE) {
		for (int i = 0; i < this->models.size(); i++) {
			MeshModel* m = dynamic_cast<MeshModel*> (this->models[i]);
			m->rotateZ(theta);
		}
	}
	else {
		MeshModel* m = dynamic_cast<MeshModel*> (this->models[active_model]);
		m->rotateZ(theta);
	}
}

void Scene::translateSelection(vec4 vec) {
	if (active_model == ALL_MODELS_ACTIVE) {
		for (int i = 0; i < this->models.size(); i++) {
			MeshModel* m = dynamic_cast<MeshModel*> (this->models[i]);
			m->translate(vec);
		}
	}
	else {
		MeshModel* m = dynamic_cast<MeshModel*> (this->models[active_model]);
		m->translate(vec);
	}
}

void Scene::activateNextModel() {
	if (models.size() == 1)	return;

	MeshModel* m = NULL;
	if (active_model == ALL_MODELS_ACTIVE) {
		active_model = 0;
		changeAllModelsActiveness(false);
		m = dynamic_cast<MeshModel*> (this->models[0]);
		m->activate();
		return;
	}

	if (active_model == models.size() - 1) {
		changeAllModelsActiveness(true);
		active_model = ALL_MODELS_ACTIVE;
	}
	else {
		active_model = (active_model + 1) % models.size();
		changeAllModelsActiveness(false);
		m = dynamic_cast<MeshModel*> (this->models[active_model]);
		m->activate();
	}
}

void Scene::activateLastModel() {
	if (models.empty())	return;
	MeshModel* m = NULL;
	for (vector<Model*>::iterator i = models.begin(); i != models.end(); i++) {
		m = dynamic_cast<MeshModel*> ((*i));
		m->deactivate();
	}
	m->activate();
	active_model = models.size() - 1;
}

void Scene::changeAllModelsActiveness(bool is_active) {
	if (models.empty())	return;
	MeshModel* m = NULL;
	for (vector<Model*>::iterator i = models.begin(); i != models.end(); i++) {
		m = dynamic_cast<MeshModel*> ((*i));
		if (is_active) {
			m->activate();
		}
		else {
			m->deactivate();
		}
	}
}

//==========


//===Display Toggles===

void Scene::togglePolyMode() {
	if (active_model == ALL_MODELS_ACTIVE) {
		for (int i = 0; i < this->models.size(); i++) {
			MeshModel* m = dynamic_cast<MeshModel*> (this->models[i]);
			m->togglePolyMode();
		}
	}
	else {
		MeshModel* m = dynamic_cast<MeshModel*> (this->models[active_model]);
		m->togglePolyMode();
	}

}


void Scene::toggleBB() {
	if (active_model == ALL_MODELS_ACTIVE) {
		for (int i = 0; i < this->models.size(); i++) {
			MeshModel* m = dynamic_cast<MeshModel*> (this->models[i]);
			m->toggleBB();
		}
	}
	else {
		MeshModel* m = dynamic_cast<MeshModel*> (this->models[active_model]);
		m->toggleBB();
	}
}

void Scene::toggleVertexNormals() {
	if (active_model == ALL_MODELS_ACTIVE) {
		for (int i = 0; i < this->models.size(); i++) {
			MeshModel* m = dynamic_cast<MeshModel*> (this->models[i]);
			m->toggleVertexNormals();
		}
	}
	else {
		MeshModel* m = dynamic_cast<MeshModel*> (this->models[active_model]);
		m->toggleVertexNormals();
	}
}

void Scene::toggleFaceNormals() {
	if (active_model == ALL_MODELS_ACTIVE) {
		for (int i = 0; i < this->models.size(); i++) {
			MeshModel* m = dynamic_cast<MeshModel*> (this->models[i]);
			m->toggleFaceNormals();
		}
	}
	else {
		MeshModel* m = dynamic_cast<MeshModel*> (this->models[active_model]);
		m->toggleFaceNormals();
	}
}

void Scene::toggleCameras() {
	f_draw_cameras = !f_draw_cameras;
}

void Scene::toggleLights() {
	f_draw_lights = !f_draw_lights;
}


//==========


//===Cameras Interface===

void Scene::addCamera(Camera* camera) {
	cameras.push_back(camera);
	activateLastCamera();
}

void Scene::activateNextCamera() {
	if (models.size() < 1)	return;
	active_camera = (active_camera + 1) % cameras.size();
	//deactivateAllCameras();
	//getActiveCamera()->setIsActive(true);
}

void Scene::activatePrevCamera() {
	if (models.size() < 1)	return;
	active_camera = (active_camera - 1);
	if (active_camera < 0)	active_camera += cameras.size();
	//deactivateAllCameras();
	//getActiveCamera()->setIsActive(true);
}

void Scene::activateLastCamera() {
	if (cameras.empty())	return;
	//deactivateAllCameras();
	//c->setIsActive(true);
	active_camera = cameras.size() - 1;
}

void Scene::activeCameraToOrtho(const float left, const float right,
	const float bottom, const float top,
	const float z_near, const float z_far) {

	getActiveCamera()->ortho(left, right, bottom, top, z_near, z_far);
}

void Scene::activeCameraToFrustum( const float left, const float right,
	const float bottom, const float top,
	const float z_near, const float z_far ) {

	getActiveCamera()->frustum(left, right, bottom, top, z_near, z_far);
}

void Scene::activeCameraToPerspective(const float fovy, const float aspect,
	const float z_near, const float z_far) {

	getActiveCamera()->perspective(fovy, aspect, z_near, z_far);
}

void Scene::lookAtActiveModel() {
	if (models.empty() || active_model == ALL_MODELS_ACTIVE) return;
	MeshModel* m = dynamic_cast<MeshModel*> (models[active_model]);
	Camera* c = getActiveCamera();
	c->lookAt(m->getPosition(), c->getUp());
}



void Scene::toggleCameraProjection() {
	getActiveCamera()->toggleProjection();
}

void Scene::rotateCameraXAroundAt(float theta) {
	getActiveCamera()->rotateXAroundAt(theta);
}

void Scene::rotateCameraYAroundAt(float theta) {
	getActiveCamera()->rotateYAroundAt(theta);
}

void Scene::rotateCameraZAroundAt(float theta) {
	getActiveCamera()->rotateZAroundAt(theta);
}

void Scene::translateCameraC(vec4 v) {
	getActiveCamera()->translateC(v);
}
void Scene::translateCameraWorld(vec4 v) {
	getActiveCamera()->translateWorld(v);
}

void Scene::rotateCameraXWorld(float theta) {
	getActiveCamera()->rotateXWorld(theta);
}
void Scene::rotateCameraYWorld(float theta) {
	getActiveCamera()->rotateYWorld(theta);
}
void Scene::rotateCameraZWorld(float theta) {
	getActiveCamera()->rotateZWorld(theta);
}



void Scene::zoom(float amount) {
	getActiveCamera()->zoom(amount);

}

//==========

//===Lights Interface===
//General

//====
//ParallelSource Interface
void Scene::addParallelSource(ParallelSource parallel_source) {
	parallel_sources.push_back(parallel_source);
}

//====

//PointSource Interface
void Scene::addPointSource(PointSource point_source) {
	point_sources.push_back(point_source);
}

//====
//==========
