#pragma once

#include "mat.h"
#include "vec.h"

class Camera {
	mat4 cTransform;
	mat4 projection;
	vec4 position;
	//bool is_active;
	vec4 up;

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
	//===========

	//===Getters/Setters===
	mat4 getTransform();
	mat4 getProjection();
	//bool getIsActive();
	//void setIsActive(bool new_is_active);
	//==========
};
