#include "stdafx.h"
#include "Scene.h"
#include "MeshModel.h"
#include "PrimMeshModel.h"
#include <string>

using namespace std;

Color camera_plus_color = { 1.0, 1.0, 0.0 };

//===Inner Getters===
Model* Scene::getActiveModel() {
	return models[active_model];
}

Camera* Scene::getActiveCamera() {
	return cameras[active_camera];
}
//==========

//===C'tors===

Scene::Scene(Renderer *renderer) : m_renderer(renderer){
	active_model = -1;
	active_camera = -1;
	Camera* def_cam = new Camera(vec4(0.0, 0.0, 10.0));
	addCamera(def_cam);
}

//==========

//===Getters/Setters===

vector<Model*> Scene::getModels() {
	return models;
}

int Scene::getActiveModelIndex() { return active_model; }

int Scene::getActiveCameraIndex() { return active_camera; }

//==========

//===Drawing Functions===

void Scene::draw()
{
	m_renderer->clearColorBuffer();
	// 1. Send the renderer the current camera transform and the projection
	m_renderer->setCameraTransform(getActiveCamera()->getTransform());
	m_renderer->setProjection(getActiveCamera()->getProjection());
	m_renderer->setWorldTransform(tw);

	// 2. Tell all models to draw themselves
	for (vector<Model*>::iterator i = models.begin(); i!=models.end(); i++){
		MeshModel* m = dynamic_cast<MeshModel*> ((*i));
		m->draw(m_renderer);
	}

	if (f_draw_cameras) {
		for (vector<Camera*>::iterator i = cameras.begin(); i != cameras.end(); i++) {
			if ((i - cameras.begin()) != active_camera) {
				vec4 pos = (*i)->getPosition();
				vec4 at = (*i)->getAt();
				vec4 up = (*i)->getUp();
				Color color = camera_plus_color;
				m_renderer->drawCamera(pos, at, up, color);
			}
		}
	}



	vector<vec4> tmp;
	tmp.push_back(vec4(0,0,0,1));
	m_renderer->drawPoints(tmp, mat4(1), Color({ 1,0,0 }));

	m_renderer->swapBuffers();
}

void Scene::drawDemo()
{
	m_renderer->SetDemoBuffer();
	m_renderer->swapBuffers();
}

//==========


//===Models Interface===

void Scene::loadOBJModel(string fileName)
{
	MeshModel *model = new MeshModel(fileName);
	models.push_back(model);
	activateLastModel();
	//Camera* c = cameras[activeCamera];

	//c->LookAt(model->getPosition());
}

void Scene::loadPrimModel(string file_name) {
	//PrimMeshModel* model = new PrimMeshModel(file_name);
	PrimMeshModel* model = new PrimMeshModel();
	models.push_back(model);
	activateLastModel();
}

void Scene::removeModel(int model) {
	if (models.empty())	return;
	if (model == ALL_MODELS_ACTIVE) {
		models.clear();
		return;
	}
	models.erase(models.begin() + model);
	activateLastModel();
}

void Scene::scaleSelection(double scale_factor) {
	if (false) {	//TODO: scale selected model

	}
	else {	//scale world
		for (int i = 0; i < this->models.size(); i++) {
			MeshModel* m = dynamic_cast<MeshModel*> (this->models[i]);
			if (m->getIsActive()) {
				m->scale(vec3(scale_factor));
			}
		}
	}
}

void Scene::scaleSelection(vec3 scale_by) {
	if (false) {	//TODO: scale selected model

	}
	else {	//scale world
		for (int i = 0; i < this->models.size(); i++) {
			MeshModel* m = dynamic_cast<MeshModel*> (this->models[i]);
			if (m->getIsActive()) {
				m->scale(scale_by);
			}
		}
	}
}

void Scene::rotateSelectionX(double theta) {
	if (false) {	//TODO: rotate selected model

	}
	else {	//rotate world
		for (int i = 0; i < this->models.size(); i++) {
			MeshModel* m = dynamic_cast<MeshModel*> (this->models[i]);
			if (m->getIsActive()) {
				m->rotateX(theta);
			}
		}
	}
}

void Scene::rotateSelectionY(double theta) {
	if (false) {	//TODO: rotate selected model

	}
	else {	//rotate world
		for (int i = 0; i < this->models.size(); i++) {
			MeshModel* m = dynamic_cast<MeshModel*> (this->models[i]);
			if (m->getIsActive()) {
				m->rotateY(theta);
			}
		}
	}
}

void Scene::rotateSelectionZ(double theta) {
	if (false) {	//TODO: rotate selected model

	}
	else {	//rotate world
		for (int i = 0; i < this->models.size(); i++) {
			MeshModel* m = dynamic_cast<MeshModel*> (this->models[i]);
			if (m->getIsActive()) {
				m->rotateZ(theta);
			}
		}
	}
}

void Scene::translateSelection(vec4 vec) {
	if (false) {	//TODO: translate selected model

	}
	else {	//translate world
		for (int i = 0; i < this->models.size(); i++) {
			MeshModel* m = dynamic_cast<MeshModel*> (this->models[i]);
			if (m->getIsActive()) {
				m->translate(vec);
			}
		}
	}
}

void Scene::activateNextModel() {
	if (models.size() == 1)	return;

	MeshModel* m = NULL;
	if (active_model == ALL_MODELS_ACTIVE) {
		active_model = 0;
		changeAllModelsActiveness(false);
		m = dynamic_cast<MeshModel*> (this->models[0]);
		m->setIsActive(true);
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
		m->setIsActive(true);
	}
}

void Scene::activateLastModel() {
	if (models.empty())	return;
	MeshModel* m = NULL;
	for (vector<Model*>::iterator i = models.begin(); i != models.end(); i++) {
		m = dynamic_cast<MeshModel*> ((*i));
		m->setIsActive(false);
	}
	m->setIsActive(true);
	active_model = models.size() - 1;
}

void Scene::changeAllModelsActiveness(bool is_active) {
	if (models.empty())	return;
	MeshModel* m = NULL;
	for (vector<Model*>::iterator i = models.begin(); i != models.end(); i++) {
		m = dynamic_cast<MeshModel*> ((*i));
		m->setIsActive(is_active);
	}
}

//==========


//===Display Toggles===

void Scene::toggleVertices() {
	if (false) {	//TODO: toggle selected model

	}
	else {	//toggle world
		for (int i = 0; i < this->models.size(); i++) {
			MeshModel* m = dynamic_cast<MeshModel*> (this->models[i]);
			m->toggleVertices();
		}
	}
}

void Scene::toggleEdges() {
	if (false) {	//TODO: toggle selected model

	}
	else {	//toggle world
		for (int i = 0; i < this->models.size(); i++) {
			MeshModel* m = dynamic_cast<MeshModel*> (this->models[i]);
			m->toggleEdges();
		}
	}
}

void Scene::toggleBB() {
	if (false) {	//TODO: toggle selected model

	}
	else {	//toggle world
		for (int i = 0; i < this->models.size(); i++) {
			MeshModel* m = dynamic_cast<MeshModel*> (this->models[i]);
			m->toggleBB();
		}
	}
}

void Scene::toggleVertexNormals() {
	if (false) {	//TODO: toggle selected model

	}
	else {	//toggle world
		for (int i = 0; i < this->models.size(); i++) {
			MeshModel* m = dynamic_cast<MeshModel*> (this->models[i]);
			m->toggleVertexNormals();
		}
	}
}

void Scene::toggleFaceNormals() {
	if (false) {	//TODO: toggle selected model

	}
	else {	//toggle world
		for (int i = 0; i < this->models.size(); i++) {
			MeshModel* m = dynamic_cast<MeshModel*> (this->models[i]);
			m->toggleFaceNormals();
		}
	}
}

void Scene::toggleCameras() {
	if (false) {	//TODO: toggle selected model

	}
	else {	//toggle world
		f_draw_cameras = !f_draw_cameras;
	}
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
	MeshModel* m = dynamic_cast<MeshModel*> (getActiveModel());
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

//===Other===
void Scene::party() {
	for (int i = 0; i < 10; i++) {
		double r = ((double)rand() / (RAND_MAX));
		double g = ((double)rand() / (RAND_MAX));
		double b = ((double)rand() / (RAND_MAX));
		Color color = { r,g,b };
		m_renderer->colorBackground(color);
		m_renderer->swapBuffers();
		Sleep(500);
	}
}
//==========