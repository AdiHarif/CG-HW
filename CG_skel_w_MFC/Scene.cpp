#include "stdafx.h"
#include "Scene.h"
#include "MeshModel.h"
#include "PrimMeshModel.h"
#include "NonUniformMeshModel.h"
#include <string>

using namespace std;

Color camera_plus_color = { 1.0, 1.0, 0.0 };

//===Inner Getters===
//Model* Scene::getActiveModel() {
//	return models[active_model];
//}

Camera* Scene::getActiveCamera() {
	return cameras[active_camera];

}
//==========

//===C'tors / Destructors===

Scene::Scene(Renderer* renderer) : m_renderer(renderer) {
	/*f_blur = false;
	f_bloom = false;
	active_model = NO_MODELS_ACTIVE;
	active_camera = -1;
	ambient_light_color = { 0.1, 0.1, 0.1 };
	light_bloom_threshold = 0.9;
	Camera* def_cam = new Camera(vec4(0.0, 0.0, 10.0));
	addCamera(def_cam);
	parallel_sources.push_back(ParallelSource("Point Light 0", vec3(0.0, 0.0, -1.0), { 0.3, 0.1, 0.1 }));
	point_sources.push_back(PointSource("Point Light 0", vec3(1.5, 1.5, 1.5), { 1, 1, 1 }));
	m_renderer->setAmbientColor(&ambient_light_color);*/


	gl_info.program = InitShader("vshader.glsl", "fshader.glsl");

}

Scene::~Scene() {
	for (vector<Camera*>::iterator i = cameras.begin(); i != cameras.end(); i++) {
		Camera* c = dynamic_cast<Camera*> ((*i));
		delete c;
	}
}
//==========

//===Getters/Setters===

//vector<Model*> Scene::getModels() {
//	return models;
//}

//int Scene::getActiveModelIndex() { return active_model; }
//
//int Scene::getActiveCameraIndex() { return active_camera; }

//==========

//===Drawing Functions===

void Scene::draw()
{
	//m_renderer->clearBuffer();

	//// 1. Send the renderer the current camera transform and the projection
	//m_renderer->setCamera(cameras[active_camera]);

	//m_renderer->point_sources = &this->point_sources;
	//m_renderer->parallel_sources = &this->parallel_sources;

	//// 2. Tell all models to draw themselves
	//for (vector<Model*>::iterator i = models.begin(); i!=models.end(); i++){
	//	MeshModel* m = dynamic_cast<MeshModel*> ((*i));
	//	m_renderer->drawModel(*m);
	//}

	//if (f_draw_cameras) {
	//	for (vector<Camera*>::iterator i = cameras.begin(); i != cameras.end(); i++) {
	//		if ((i - cameras.begin()) != active_camera) {
	//			vec4 pos = (*i)->getPosition();
	//			Color color = camera_plus_color;
	//			m_renderer->drawCamera(pos, color);
	//		}
	//	}
	//}

	//if (f_draw_lights) {
	//	for (vector<PointSource>::iterator i = point_sources.begin(); i != point_sources.end(); i++) {
	//		m_renderer->drawLight(*i);
	//	}
	//}

	//m_renderer->drawOrigin(Color(RED));

	//if (f_blur) {
	//	m_renderer->applyBlur(m_renderer->m_outBuffer);
	//}

	//if (f_bloom) {
	//	m_renderer->applyBloom(light_bloom_threshold);
	//}

	//m_renderer->drawAxes();
	//m_renderer->swapBuffers();

	/*glGenVertexArrays(1, &gl_info.vao);
	glBindVertexArray(gl_info.vao);*/
	

	


	GLuint loc = glGetAttribLocation(gl_info.program, "vPosition");
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
	glBindVertexArray(m->vao);
	glDrawArrays(GL_LINE_LOOP, 0, m->vertices.size());
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

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, model->vertices.size()*sizeof(vec4),&model->vertices[0], GL_STATIC_DRAW);


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

//void Scene::toggleVertices() {
//	if (false) {	//TODO: toggle selected model
//
//	}
//	else {	//toggle world
//		for (int i = 0; i < this->models.size(); i++) {
//			MeshModel* m = dynamic_cast<MeshModel*> (this->models[i]);
//			m->toggleVertices();
//		}
//	}
//}
//
//void Scene::toggleEdges() {
//	if (false) {	//TODO: toggle selected model
//
//	}
//	else {	//toggle world
//		for (int i = 0; i < this->models.size(); i++) {
//			MeshModel* m = dynamic_cast<MeshModel*> (this->models[i]);
//			m->toggleEdges();
//		}
//	}
//}

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


//void Scene::deactivateAllCameras() {
//	Camera* c = NULL;
//	for (vector<Camera*>::iterator i = cameras.begin(); i != cameras.end(); i++) {
//		c = dynamic_cast<Camera*> ((*i));
//		c->setIsActive(false);
//	}
//}

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

void Scene::toggleAntiAliasing() {
	//m_renderer->toggleAntiAliasing();
}

void Scene::toggleFog() {
	//m_renderer->toggleFog();
}

void Scene::toggleBlur() {
	f_blur = !f_blur;
}

void Scene::toggleBloom() {
	f_bloom = !f_bloom;
}

void Scene::updateActiveModelFacesColors() {

}