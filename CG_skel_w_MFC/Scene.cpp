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

	glBlendFunc(GL_ONE, GL_ONE);

	active_model = NO_MODELS_ACTIVE;

	active_camera = -1;
	Camera* def_cam = new Camera(vec4(0.0, 0.0, 10.0));
	addCamera(def_cam);

	ambient_light_color = { 0.1, 0.1, 0.1 };
	parallel_sources.push_back(ParallelSource("Parallel Light 0", vec3(0.0, 1.0, 0.0), { 0.4, 0, 0 }));
	point_sources.push_back(PointSource("Point Light 0", vec3(1.5, 1.5, 1.5), { 1, 1, 1 }));

	ambient_programs[AMBIENT] = InitShader("ambient_vshader.glsl", "ambient_fshader.glsl");
	active_ambient_method = AMBIENT;

	shading_programs[FLAT_SHADING] = InitShader("flat_vshader.glsl", "flat_fshader.glsl");
	shading_programs[GOURAUD_SHADING] = InitShader("gouraud_vshader.glsl", "gouraud_fshader.glsl");
	//TODO: add initializing of other shaders
	active_shading_method = FLAT_SHADING;
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
	GLuint active_program = shading_programs[active_shading_method];

	glClearColor(0.2, 0.2, 0.2, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (vector<Model*>::iterator i = models.begin(); i != models.end(); i++) {
		
		setupAmbientProgram((MeshModel*)*i);
		(*i)->draw();

		//here would be the for loop going through lights

		glEnable(GL_BLEND);
		glDepthFunc(GL_EQUAL);

		for (vector<ParallelSource>::iterator j = parallel_sources.begin(); j != parallel_sources.end(); j++) {
			setupShadingProgram((MeshModel*)*i, &(ParallelSource)*j);
			(*i)->draw();
		}
		for (vector<PointSource>::iterator j = point_sources.begin(); j != point_sources.end(); j++) {
			setupShadingProgram((MeshModel*)*i, &(PointSource)*j);
			(*i)->draw();
		}


		glDisable(GL_BLEND);
		glDepthFunc(GL_LESS);
	}

	//lighting:

	//parallel lights:
	//for (vector<ParallelSource>::iterator i = parallel_sources.begin(); i != parallel_sources.end(); i++) {
	//	for (vector<Model*>::iterator j = models.begin(); j != models.end(); j++) {
	//		bindAttributesToProgram((MeshModel*)*j, active_program);
	//		(*j)->draw(tpc, active_program);
	//	}
	//}

	glutSwapBuffers();
}

//==========


//===Models Interface===

void Scene::loadOBJModel(string fileName)
{
	MeshModel *model = new MeshModel(fileName);
	models.push_back(model);
	activateLastModel();
}

void Scene::loadPrimModel() {
	PrimMeshModel* model = new PrimMeshModel();
	models.push_back(model);
	activateLastModel();
}

void Scene::loadNonUniform() {
	/*NonUniformMeshModel* model = new NonUniformMeshModel();
	models.push_back(model);*/
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

void Scene::toggleShadingMethod() {
	active_shading_method = ShadingMethod((active_shading_method + 1) % 2);//SHADING_METHODS_COUNT);
	//glUseProgram(programs[active_shading_method]);
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


//===OpenGL===

void Scene::setupAmbientProgram(MeshModel* m) {

	glBindVertexArray(m->vao);
	GLuint program = ambient_programs[active_ambient_method];
	glUseProgram(program);

	Camera* c = cameras[active_camera];

	mat4 vt = c->tp * c->tc * m->tw * m->tm;

	GLuint vt_loc = glGetUniformLocation(program, "v_transform");
	glUniformMatrix4fv(vt_loc, 1, GL_TRUE, vt);

	switch (active_ambient_method) {
	case AMBIENT:
		bindBufferToProgram(m, program, m->vbos[BT_VERTICES], "v_position", GL_FALSE);
		break;
	case TEXTURE:
		//TODO: implement
		break;
	}
}

void Scene::setupShadingProgram(MeshModel* m, Light* l) { // TODO: add face colors

	glBindVertexArray(m->vao);
	GLuint program = shading_programs[active_shading_method];
	glUseProgram(program);

	Camera* c = cameras[active_camera];

	mat4 vt = c->tp * c->tc * m->tw * m->tm;
	mat4 nt = m->ntw * m->ntm;

	GLuint vt_loc = glGetUniformLocation(program, "v_transform");
	glUniformMatrix4fv(vt_loc, 1, GL_TRUE, vt);

	GLuint nt_loc = glGetUniformLocation(program, "n_transform");
	glUniformMatrix4fv(nt_loc, 1, GL_TRUE, nt);

	vec4 light_color = vec4(l->getColor().r, l->getColor().g, l->getColor().b, 1);
	GLuint light_color_loc = glGetUniformLocation(program, "light_color");
	glUniform4fv(light_color_loc, 1, light_color);

	PointSource* poi_s = dynamic_cast<PointSource*>(l);
	if (poi_s) {
		GLuint light_pos_loc = glGetUniformLocation(program, "light_pos");
		glUniform4fv(light_pos_loc, 1, vec4(poi_s->getPosition(), 1));

		GLuint is_point_source_loc = glGetUniformLocation(program, "is_point_source");
		glUniform1i(is_point_source_loc, GL_TRUE);
	}

	ParallelSource* par_s = dynamic_cast<ParallelSource*>(l);
	if (par_s) {
		GLuint light_dir_loc = glGetUniformLocation(program, "light_dir");
		glUniform4fv(light_dir_loc, 1, vec4(par_s->getDirection(), 1));

		GLuint is_point_source_loc = glGetUniformLocation(program, "is_point_source");
		glUniform1i(is_point_source_loc, GL_FALSE);
	}

	GLuint camera_pos_loc = glGetUniformLocation(program, "camera_pos");
	glUniform4fv(camera_pos_loc, 1, c->position);

	vec4 model_diffuse_color = vec4(m->diffuse_color.r, m->diffuse_color.g, m->diffuse_color.b, 1);
	GLuint model_diffuse_color_loc = glGetUniformLocation(program, "model_diffuse_color");
	glUniform4fv(model_diffuse_color_loc, 1, model_diffuse_color);

	vec4 model_specular_color = vec4(m->specular_color.r, m->specular_color.g, m->specular_color.b, 1);
	GLuint model_specular_color_loc = glGetUniformLocation(program, "model_specular_color");
	glUniform4fv(model_specular_color_loc, 1, model_specular_color);

	GLuint shininess_loc = glGetUniformLocation(program, "shininess");
	glUniform1f(shininess_loc, m->shininess);

	switch (active_shading_method) {
	case FLAT_SHADING:
		bindBufferToProgram(m, program, m->vbos[BT_VERTICES], "v_position", GL_FALSE);
		bindBufferToProgram(m, program, m->vbos[BT_FACE_NORMALS], "f_normal", GL_TRUE);
		break;
	case GOURAUD_SHADING:
		bindBufferToProgram(m, program, m->vbos[BT_VERTICES], "v_position", GL_FALSE);
		bindBufferToProgram(m, program, m->vbos[BT_VERTEX_NORMALS], "v_normal", GL_TRUE);
		break;
	case PHONG_SHADING:
		//TODO: implement
		break;
	}
}

//void Scene::bindAttributesToProgram(MeshModel* model, GLuint program) {
//	glBindVertexArray(model->vao);
//	bindAttributeToProgram(model, program, model->vbos[BT_VERTICES], "v_position", GL_FALSE);
//	bindAttributeToProgram(model, program, model->vbos[BT_VERTEX_NORMALS], "v_normal", GL_TRUE);
//	bindAttributeToProgram(model, program, model->vbos[BT_FACE_NORMALS], "f_normal", GL_TRUE);
//	bindAttributeToProgram(model, program, model->vbos[BT_TEXTURES], "texture", GL_FALSE);
//}

void Scene::bindBufferToProgram(MeshModel* model, GLuint program, GLuint vbo, GLchar* variable_name, boolean is_normalized) {
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	GLuint loc = glGetAttribLocation(program, variable_name);
	glEnableVertexAttribArray(loc);
	glVertexAttribPointer(loc, 4, GL_FLOAT, is_normalized, 0, 0);
}

//==========
