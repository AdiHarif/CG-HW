#include "StdAfx.h"
#include "Camera.h"


//===Inner Calculations===

vec4 Camera::calcUp(vec4 at) {
    vec4 z = vec4(0.0, 0.0, 1.0, 1.0);
    if (position == at) return z;
    vec4 v = at - position;
    if (v * z == 0) return z;
    if (length(cross(v, z)) < FLT_EPSILON)	return vec4(0.0, 1.0, 0.0, 1.0);
    up = cross(v, cross(z, v));
    return up;
}

//==========


//===C'tor===

Camera::Camera(vec4 position) {
    this->position = position;
    lookAt(vec4(0.0));
    ortho(-3.0, 3.0, -3.0, 3.0, 3.0, -3.0);
}

//==========


//===Transformations Interface===

void Camera::lookAt(const vec4& at) {
    up = calcUp(at);
    vec4 n = normalize(position - at);
    vec4 u = normalize(cross(up, n));
    vec4 v = normalize(cross(n, u));
    vec4 t = vec4(0.0, 0.0, 0.0, 1.0);
    mat4 c = mat4(u, v, n, t);
    cTransform = c * translateMat(-position);
}

//==========


//===Projections===
void Camera::ortho(const float left, const float right,
    const float bottom, const float top,
    const float z_near, const float z_far) {

    mat4 p;
    p[2][2] = 0;
    mat4 t = translateMat(-(right + left) / 2, -(bottom + top) / 2, (z_near + z_far) / 2);
    mat4 s = scaleMat(2 / (right - left), 2 / (top - bottom), 2 / (z_near - z_far));
    projection = p * s * t;
}


void Camera::frustum(const float left, const float right,
    const float bottom, const float top,
    const float z_near, const float z_far) {
    mat4 p;
    p[2][2] = 0;
    mat4 h;
    h[0][2] = (left + right) / 2;
    h[1][2] = (top + bottom) / 2;
    mat4 s = scaleMat((2 * z_near) / (right - left), (2 * z_near) / (top - bottom), 1.0);
    mat4 n;
    n[2][2] = (z_near + z_far) / (z_near - z_far);
    n[2][3] = (2 * z_near * z_far) / (z_near - z_far);
    projection = p * n * s * h;
}
//===========


//===Getters===

mat4 Camera::getTransform() { return cTransform; }

mat4 Camera::getProjection() { return projection; }

//bool Camera::getIsActive() { return is_active; }

//void Camera::setIsActive(bool new_is_active) { is_active = new_is_active; }
//==========