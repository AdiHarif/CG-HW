#include "stdafx.h"
#include "Scene.h"
#include "MeshModel.h"
#include <string>

using namespace std;


//===C'tors===

Scene::Scene(Renderer *renderer) : m_renderer(renderer){
	active_model = -1;
	active_camera = -1;
	Camera* def_cam =new Camera(vec4(0.0, 0.0, 10.0));
	addCamera(def_cam);
}

//==========

//===Getters/Setters===

vector<Model*> Scene::getModels() {
	return models;
}

int Scene::getActiveModelIndex() {
	return active_model;
}

//==========

//===Drawing Functions===

void Scene::draw()
{
	// 1. Send the renderer the current camera transform and the projection
	// 2. Tell all models to draw themselves
	m_renderer->clearColorBuffer();
	mat4 tvp = m_renderer->getViewport();
	mat4 tc = cameras[active_camera]->getTransform();
	mat4 tp = cameras[active_camera]->getProjection();
	mat4 t_tot = tvp* tp * tc;
	for (vector<Model*>::iterator i = models.begin(); i!=models.end(); i++){
		MeshModel* m = dynamic_cast<MeshModel*> ((*i));
		m->draw(t_tot, m_renderer);
		//m->drawVertices(t_tot, m_renderer);
		//m->drawEdges(t_tot, m_renderer);
		//m->drawBoundingBox(t_tot, m_renderer);
	}

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
	//activeModel = models.size()-1;
	//Camera* c = cameras[activeCamera];

	//c->LookAt(model->getPosition());
}

void Scene::scaleSelection(double scale_factor) {
	if (false) {	//TODO: scale selected model

	}
	else {	//scale world
		for (int i = 0; i < this->models.size(); i++) {
			MeshModel* m = dynamic_cast<MeshModel*> (this->models[i]);
			m->scale(vec3(scale_factor));
		}
	}
}

void Scene::rotateSelectionX(double theta) {
	if (false) {	//TODO: rotate selected model

	}
	else {	//rotate world
		for (int i = 0; i < this->models.size(); i++) {
			MeshModel* m = dynamic_cast<MeshModel*> (this->models[i]);
			m->rotateX(theta);
		}
	}
}

void Scene::rotateSelectionY(double theta) {
	if (false) {	//TODO: rotate selected model

	}
	else {	//rotate world
		for (int i = 0; i < this->models.size(); i++) {
			MeshModel* m = dynamic_cast<MeshModel*> (this->models[i]);
			m->rotateY(theta);
		}
	}
}

void Scene::rotateSelectionZ(double theta) {
	if (false) {	//TODO: rotate selected model

	}
	else {	//rotate world
		for (int i = 0; i < this->models.size(); i++) {
			MeshModel* m = dynamic_cast<MeshModel*> (this->models[i]);
			m->rotateZ(theta);
		}
	}
}

void Scene::translateSelection(vec4 vec) {
	if (false) {	//TODO: translate selected model

	}
	else {	//translate world
		for (int i = 0; i < this->models.size(); i++) {
			MeshModel* m = dynamic_cast<MeshModel*> (this->models[i]);
			m->translate(vec);
		}
	}
}

void Scene::activateNextModel() {
	if (models.size() == 1)	return;

	MeshModel* m = NULL;
	if (active_model == ALL_MODELS_ACTIVE) {
		active_model = 0;
		for (vector<Model*>::iterator i = models.begin(); i != models.end(); i++) {
			m = dynamic_cast<MeshModel*> ((*i));
			m->setIsActive(false);
		}
		m = dynamic_cast<MeshModel*> (this->models[0]);
		m->setIsActive(true);
		return;
	}

	if (active_model == models.size() - 1) {
		for (vector<Model*>::iterator i = models.begin(); i != models.end(); i++) {
			m = dynamic_cast<MeshModel*> ((*i));
			m->setIsActive(true);
		}
		active_model = ALL_MODELS_ACTIVE;
	}
	else {
		active_model = (active_model + 1) % models.size();
		for (vector<Model*>::iterator i = models.begin(); i != models.end(); i++) {
			m = dynamic_cast<MeshModel*> ((*i));
			m->setIsActive(false);
		}
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

//==========


//===Cameras Interface===

void Scene::addCamera(Camera* camera) {
	cameras.push_back(camera);
	active_camera = cameras.size() - 1;
}

//==========
