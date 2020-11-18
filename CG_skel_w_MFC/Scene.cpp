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


void Camera::lookAt(const vec4& at){
	vec4 up = calcUp(at);
	vec4 n = normalize(position - at);
    vec4 u = normalize(cross(up, n));
    vec4 v = normalize(cross(n, u));
    vec4 t = vec4(0.0, 0.0, 0.0, 1.0);
    mat4 c = mat4(u, v, n, t);
    cTransform = c * translateMat(-position);
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


vec4 Camera::calcUp(vec4 at){
	vec4 z = vec4(0.0, 0.0, 1.0, 1.0);
	if (position == at) return z;
	vec4 v = at-position;
	if (v*z == 0) return z;
	if (length(cross(v, z)) < FLT_EPSILON)	return vec4(0.0, 1.0, 0.0, 1.0);
	return cross(v, cross(z, v));
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

Camera::Camera(vec4 position){
	this->position = position;
	lookAt(vec4(0.0));
	ortho(-3.0, 3.0, -3.0, 3.0, 3.0, -3.0);
}

mat4 Camera::getTransform(){ return cTransform; }


void Camera::ortho( const float left, const float right,
		const float bottom, const float top, 
		const float z_near, const float z_far ){

    mat4 p;
    p[2][2] = 0;
	mat4 t = translateMat(-(right + left) / 2, -(bottom + top) / 2, (z_near + z_far) / 2);
	mat4 s = scaleMat(2 / (right - left), 2 / (top - bottom), 2 / (z_near - z_far));
    projection = p*s*t;
}

void Camera::frustum( const float left, const float right,
		const float bottom, const float top,
		const float z_near, const float z_far ){
    mat4 p;
    p[2][2] = 0;
    mat4 h;
    h[0][2] = (left+right)/2;
    h[1][2] = (top+bottom)/2;
    mat4 s = scaleMat((2*z_near)/(right-left), (2*z_near)/(top-bottom), 1.0);
    mat4 n;
    n[2][2] = (z_near+z_far)/(z_near-z_far);
    n[2][3] = (2*z_near*z_far)/(z_near-z_far);
    projection = p*n*s*h;
}

mat4 Camera::getProjection() { return projection; }