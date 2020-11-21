#pragma once

#include "mat.h"
#include "vec.h"

enum proj_type { ortho = 0, frustum = 1 };

class Camera {
	mat4 tc;
	mat4 tp;
	vec4 position;
	//bool is_active;
	vec4 up;

	float left;
	float right;
	float bottom; 
	float top;
	float z_near;
	float z_far;


	enum proj_type { p_ortho = 0, p_persp = 1 } projection;

	vec4 calcUp(vec4 at);

public:
	//===C'tor===
	Camera(vec4 position);
	//==========

	//Transformations Interface===
	void setTransformation(const mat4& transform);
	//==========

	//===Projections===
	void lookAt(const vec4& at);
	void ortho(const float left, const float right,
		const float bottom, const float top,
		const float z_near, const float z_far);
	void frustum(const float left, const float right,
		const float bottom, const float top,
		const float z_near, const float z_far);
	mat4 perspective(const float fovy, const float aspect,
		const float z_near, const float z_far);

	void toggleProjection();
	//===========

	//===Getters/Setters===
	mat4 getTransform();
	mat4 getProjection();
	//bool getIsActive();
	//void setIsActive(bool new_is_active);
	//==========
};
