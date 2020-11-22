#pragma once

#include "mat.h"
#include "vec.h"

enum proj_type { ortho = 0, frustum = 1 };

class Camera {
	mat4 tc;
	mat4 tp;

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

	//===Inner Calculations===
	mat4 calcTcInverse();
	//==========

public:
	//===C'tor===
	Camera(vec4 position);
	//==========

	//Transformations Interface===
	void setTransformation(vec4 x_axis, vec4 y_axis, vec4 z_axis, vec4 position);

	void rotateYAroundAt(float theta);

	void translate(vec4 v);



	void lookAt();
	void lookAt(const vec4 at, vec4 up = vec4(0, 1, 0, 1));
	//==========

	//===Projections===
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
