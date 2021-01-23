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
	point_sources.push_back(PointSource("Point Light 0", vec3(4, 4, 4), { 0.3, 0.3, 0.3 }));
	//point_sources.push_back(PointSource("Point Light 1", vec3(-1, 0, 0), { 0, 0, 1 }));

	ambient_programs[AMBIENT] = InitShader("ambient_vshader.glsl", "ambient_fshader.glsl");
	ambient_programs[TEXTURE] = InitShader("texture_vshader.glsl", "texture_fshader.glsl");
	ambient_programs[PLANE_TEXTURE] = InitShader("plane_texture_projection_vshader.glsl", "plane_texture_projection_fshader.glsl");
	ambient_programs[SPHERE_TEXTURE] = InitShader("sphere_texture_projection_vshader.glsl", "sphere_texture_projection_fshader.glsl");
	active_ambient_method = AMBIENT;

	shading_programs[FLAT_SHADING] = InitShader("flat_vshader.glsl", "flat_fshader.glsl");
	shading_programs[GOURAUD_SHADING] = InitShader("gouraud_vshader.glsl", "gouraud_fshader.glsl");
	shading_programs[PHONG_SHADING] = InitShader("phong_vshader.glsl", "phong_fshader.glsl");
	shading_programs[TOON_SHADING] = InitShader("phong_vshader.glsl", "toon_fshader.glsl");
	active_shading_method = FLAT_SHADING;

	special_programs[SILHOUETTE] = InitShader("silhouette_vshader.glsl", "silhouette_fshader.glsl");

	color_animation_programs[SMOOTH] = InitShader("color_animation_smooth_vshader.glsl", "color_animation_smooth_fshader.glsl");
	color_animation_programs[WAVE] = InitShader("color_animation_wave_vshader.glsl", "color_animation_wave_fshader.glsl");
	active_color_animation_method = SMOOTH;

	vertex_animation_programs[SUPER_NOVA] = InitShader("vertex_animation_vshader.glsl", "vertex_animation_fshader.glsl");
	active_vertex_animation_method = SUPER_NOVA;
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

	glClearColor(0.2, 0.2, 0.2, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (vector<Model*>::iterator i = models.begin(); i != models.end(); i++) {

		MeshModel* m = dynamic_cast<MeshModel*> ((*i));
		if (active_shading_method == TOON_SHADING && m->draw_pref.poly_mode == DrawPref::PolyMode::FILLED) {
			setupSpecialProgram(m, SILHOUETTE);
			m->draw();
		}
		
		if (!f_color_animation_active && !f_vertex_animation_active) {
			setupAmbientProgram(m);
			m->draw();
		}
		else {
			if (f_color_animation_active) {
				setupColorAnimationProgram(m);
				m->draw();
			}
			if (f_vertex_animation_active) {
				setupVertexAnimationProgram(m);
				m->draw();
			}
		}

		glEnable(GL_BLEND);
		glDepthFunc(GL_EQUAL);

		for (vector<ParallelSource>::iterator j = parallel_sources.begin(); j != parallel_sources.end(); j++) {
			setupShadingProgram(m, &(ParallelSource)*j);
			m->draw();
		}
		for (vector<PointSource>::iterator j = point_sources.begin(); j != point_sources.end(); j++) {
			setupShadingProgram(m, &(PointSource)*j);
			m->draw();
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

void Scene::loadTexture(string fileName) {
	if (active_model == NO_MODELS_ACTIVE)	return;
	for (vector<Model*>::iterator i = models.begin(); i != models.end(); i++) {
		MeshModel* m = dynamic_cast<MeshModel*>(*i);
		m->setTexture(fileName.c_str());
	}
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
	active_shading_method = ShadingMethod((active_shading_method + 1) % SHADING_METHODS_COUNT);
	//glUseProgram(programs[active_shading_method]);
}

void Scene::toggleAmbientMethod() {
	active_ambient_method = AmbientMethod((active_ambient_method + 1) % AMBIENT_METHODS_COUNT);
}

void Scene::toggleColorAnimationMethod() {
	active_color_animation_method = ColorAnimationMethod((active_color_animation_method + 1) % COLOR_ANIMATIONS_METHODS_COUNT);
}


void Scene::toggleIsColorAnimationActive() {
	f_color_animation_active = !f_color_animation_active;

}

void Scene::toggleIsVertexAnimationActive() {
	f_vertex_animation_active = !f_vertex_animation_active;
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

	bindBufferToProgram(m, program, m->vbos[BT_VERTICES], "v_position", GL_FALSE);
	GLuint model_texture_loc;

	switch (active_ambient_method) {
	case AMBIENT:
		break;
	case TEXTURE: {
		model_texture_loc = glGetUniformLocation(program, "modelTexture");
		glUniform1i(model_texture_loc, 0);

		glBindBuffer(GL_ARRAY_BUFFER, m->vbos[BT_TEXTURES]);
		GLuint tex_coord_loc = glGetAttribLocation(program, "tex_coord");
		glEnableVertexAttribArray(tex_coord_loc);
		glVertexAttribPointer(tex_coord_loc, 2, GL_FLOAT, GL_TRUE, 0, 0);

		glBindTexture(GL_TEXTURE_2D, m->vto);
		break;
	}
	case PLANE_TEXTURE: {
		model_texture_loc = glGetUniformLocation(program, "modelTexture");
		glUniform1i(model_texture_loc, 0);
		
		glBindTexture(GL_TEXTURE_2D, m->vto);
		break;
	}
	case SPHERE_TEXTURE: {
		model_texture_loc = glGetUniformLocation(program, "modelTexture");
		glUniform1i(model_texture_loc, 0);

		glBindTexture(GL_TEXTURE_2D, m->vto);
		break;
	}
	}
}

void Scene::setupColorAnimationProgram(MeshModel* m) {
	glBindVertexArray(m->vao);
	GLuint program = color_animation_programs[active_color_animation_method];
	glUseProgram(program);

	Camera* c = cameras[active_camera];

	mat4 vt = c->tp * c->tc * m->tw * m->tm;

	GLuint vt_loc = glGetUniformLocation(program, "v_transform");
	glUniformMatrix4fv(vt_loc, 1, GL_TRUE, vt);
	
	bindBufferToProgram(m, program, m->vbos[BT_VERTICES], "v_position", GL_FALSE);

	switch (active_color_animation_method) {
	case SMOOTH: {
		GLuint hsv_color_loc = glGetUniformLocation(program, "hsv_color");
		glUniform3f(hsv_color_loc, m->hsv_color.r, m->hsv_color.g, m->hsv_color.b);
		break;
	}
	case WAVE: {
		GLuint color0_loc = glGetUniformLocation(program, "color0");
		glUniform3f(color0_loc, m->wave_color_0.r, m->wave_color_0.g, m->wave_color_0.b);

		GLuint color1_loc = glGetUniformLocation(program, "color1");
		glUniform3f(color1_loc, m->wave_color_1.r, m->wave_color_1.g, m->wave_color_1.b);

		GLuint wave_threshold_loc = glGetUniformLocation(program, "wave_threshold");
		glUniform1f(wave_threshold_loc, m->wave_threshold);
		break;
	}
	}
}

void Scene::setupVertexAnimationProgram(MeshModel* m) {
	glBindVertexArray(m->vao);
	GLuint program = color_animation_programs[active_color_animation_method];
	glUseProgram(program);

	Camera* c = cameras[active_camera];

	mat4 vt = c->tp * c->tc * m->tw * m->tm;

	GLuint vt_loc = glGetUniformLocation(program, "v_transform");
	glUniformMatrix4fv(vt_loc, 1, GL_TRUE, vt);

	bindBufferToProgram(m, program, m->vbos[BT_VERTICES], "v_position", GL_FALSE);

	switch (active_vertex_animation_method) {
	case SUPER_NOVA:
		GLuint vertex_animation_t_loc = glGetUniformLocation(program, "vertex_animation_t");
		glUniform1f(vertex_animation_t_loc, m->vertex_animation_t);
		break;
	}
}

void Scene::setupShadingProgram(MeshModel* m, Light* l) {
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


	bindBufferToProgram(m, program, m->vbos[BT_VERTICES], "v_position", GL_FALSE);

	switch (active_shading_method) {
	case FLAT_SHADING:
		bindBufferToProgram(m, program, m->vbos[BT_FACE_NORMALS], "f_normal", GL_TRUE);
		break;
	case GOURAUD_SHADING:
		bindBufferToProgram(m, program, m->vbos[BT_VERTEX_NORMALS], "v_normal", GL_TRUE);
		break;
	case PHONG_SHADING:
	case TOON_SHADING:
		bindBufferToProgram(m, program, m->vbos[BT_VERTEX_NORMALS], "v_normal", GL_TRUE);
		break;
	}
}


void Scene::setupSpecialProgram(MeshModel* m, SpecialShaders shader) {
	glBindVertexArray(m->vao);
	GLuint program = special_programs[shader];
	glUseProgram(program);

	Camera* c = cameras[active_camera];

	mat4 twm = m->tw * m->tm;
	mat4 tpc = c->tp * c->tc;
	mat4 twm_n = m->ntw * m->ntm;

	GLuint twm_loc = glGetUniformLocation(program, "twm");
	glUniformMatrix4fv(twm_loc, 1, GL_TRUE, twm);

	GLuint tpc_loc = glGetUniformLocation(program, "tpc");
	glUniformMatrix4fv(tpc_loc, 1, GL_TRUE, tpc);

	GLuint twm_n_loc = glGetUniformLocation(program, "twm_n");
	glUniformMatrix4fv(twm_n_loc, 1, GL_TRUE, twm_n);

	switch (shader) {
	case SILHOUETTE:
		bindBufferToProgram(m, program, m->vbos[BT_VERTICES], "v_position", GL_FALSE);
		bindBufferToProgram(m, program, m->vbos[BT_VERTEX_NORMALS], "v_normal", GL_TRUE);
		break;
	}
}


void Scene::bindBufferToProgram(MeshModel* model, GLuint program, GLuint vbo, GLchar* variable_name, boolean is_normalized) {
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	GLuint loc = glGetAttribLocation(program, variable_name);
	glEnableVertexAttribArray(loc);
	glVertexAttribPointer(loc, 4, GL_FLOAT, is_normalized, 0, 0);
}

//==========


//===Animations===

bool Scene::getIsColorAnimationActive() {
	return f_color_animation_active;
}

bool Scene::getIsVertexAnimationActive() {
	return f_vertex_animation_active;
}

ColorAnimationMethod Scene::getActiveColorAnimationMethod() {
	return active_color_animation_method;
}

VertexAnimationMethod Scene::getActiveVertexAnimationMethod() {
	return active_vertex_animation_method;
}

void Scene::updateAllModelsHSVColor() {
	for (vector<Model*>::iterator i = models.begin(); i != models.end(); i++) {
		MeshModel* m = dynamic_cast<MeshModel*>(*i);
		m->updateHSVColor();
	}
}

void Scene::updateAllModelsWaveThreshold() {
	for (vector<Model*>::iterator i = models.begin(); i != models.end(); i++) {
		MeshModel* m = dynamic_cast<MeshModel*>(*i);
		m->updateWaveThreshold();
	}
}

void Scene::updateAllModelsVertexAnimationT() {
	for (vector<Model*>::iterator i = models.begin(); i != models.end(); i++) {
		MeshModel* m = dynamic_cast<MeshModel*>(*i);
		m->updateVertexAnimationT();
	}
}

//==========