#pragma once

#include "mat.h"
#include "vec.h"

enum proj_type { ortho = 0, frustum = 1 };

class Camera {
	mat4 tc;
	mat4 tp;

	mat4 tcn;
	mat4 tpn;

	mat4 tci;

	vec4 position;
	vec4 at;
	vec4 up;

	float left;
	float right;
	float bottom; 
	float top;
	float z_near;
	float z_far;


	enum proj_type { p_ortho = 0, p_persp = 1 } projection;

	

public:
	//===C'tor===
	Camera(vec4 position);
	Camera(vec4 position, vec4 at);
	Camera(vec4 position, vec4 at, vec4 up);
	//==========

	//Transformations Interface===
	void setTransformation(vec4 x_axis, vec4 y_axis, vec4 z_axis, vec4 position);

	void rotateXAroundAt(float theta);
	void rotateYAroundAt(float theta);
	void rotateZAroundAt(float theta);

	void translateC(vec4 v); //translate in camera coordinates
	void translateWorld(vec4 v); //translate in world coordinates

	void rotateXWorld(float theta);
	void rotateYWorld(float theta);
	void rotateZWorld(float theta);

	void lookAt();
	void lookAt(const vec4 at, vec4 up = vec4(0, 1, 0, 1));

	void zoom(float aspect);
	//==========

	//===Projections===
	void ortho(const float left, const float right,
		const float bottom, const float top,
		const float z_near, const float z_far);
	void frustum(const float left, const float right,
		const float bottom, const float top,
		const float z_near, const float z_far);
	void perspective(const float fovy, const float aspect,
		const float z_near, const float z_far);

	void toggleProjection();
	//===========

	//===Getters/Setters===
	mat4 getTransform();
	mat4 getProjection();
	vec4 getPosition();
	vec4 getAt();
	vec4 getUp();
	//==========

	friend class Renderer;
	friend class Scene;

};
