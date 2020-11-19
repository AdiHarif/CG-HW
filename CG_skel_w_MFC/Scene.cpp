#include "stdafx.h"
#include "Scene.h"
#include "MeshModel.h"
#include <string>

using namespace std;


Scene::Scene(Renderer *renderer) : m_renderer(renderer){
	activeModel = -1;
	activeCamera = -1;
	Camera* def_cam =new Camera(vec4(0.0, 0.0, 10.0));
	addCamera(def_cam);
}



void Scene::loadOBJModel(string fileName)
{
	MeshModel *model = new MeshModel(fileName);
	models.push_back(model);
	//activeModel = models.size()-1;
	//Camera* c = cameras[activeCamera];

	//c->LookAt(model->getPosition());
}

void Scene::draw()
{
	// 1. Send the renderer the current camera transform and the projection
	// 2. Tell all models to draw themselves
	m_renderer->clearColorBuffer();
	mat4 tvp = m_renderer->getViewport();
	mat4 tc = cameras[activeCamera]->getTransform();
	mat4 tp = cameras[activeCamera]->getProjection();
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



void Scene::scale(double scale_factor) {
	if (false) {	//TODO: scale selected model

	}
	else {	//scale world
		for (int i = 0; i < this->models.size(); i++) {
			MeshModel* m = dynamic_cast<MeshModel*> (this->models[i]);
			m->scale(vec3(scale_factor));
		}
	}
}

void Scene::rotateX(double theta) {
	if (false) {	//TODO: rotate selected model

	}
	else {	//rotate world
		for (int i = 0; i < this->models.size(); i++) {
			MeshModel* m = dynamic_cast<MeshModel*> (this->models[i]);
			m->rotateX(theta);
		}
	}
}

void Scene::rotateY(double theta) {
	if (false) {	//TODO: rotate selected model

	}
	else {	//rotate world
		for (int i = 0; i < this->models.size(); i++) {
			MeshModel* m = dynamic_cast<MeshModel*> (this->models[i]);
			m->rotateY(theta);
		}
	}
}

void Scene::rotateZ(double theta) {
	if (false) {	//TODO: rotate selected model

	}
	else {	//rotate world
		for (int i = 0; i < this->models.size(); i++) {
			MeshModel* m = dynamic_cast<MeshModel*> (this->models[i]);
			m->rotateZ(theta);
		}
	}
}

void Scene::translate(vec4 vec) {
	if (false) {	//TODO: translate selected model

	}
	else {	//translate world
		for (int i = 0; i < this->models.size(); i++) {
			MeshModel* m = dynamic_cast<MeshModel*> (this->models[i]);
			m->translate(vec);
		}
	}
}

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


void Scene::addCamera(Camera* camera) {
	cameras.push_back(camera);
	activeCamera = cameras.size() - 1;
}
