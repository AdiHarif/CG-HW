#include "StdAfx.h"
#include "Camera.h"




//===C'tor===

Camera::Camera(vec4 position) {
    this->position = position;
    lookAt(vec4(0,0,0,1));
    ortho(-3.0, 3.0, -3.0, 3.0, -5.0, -15.0);
}

//==========


//===Transformations Interface===


void Camera::setTransformation(vec4 x_axis, vec4 y_axis, vec4 z_axis, vec4 position) {

    x_axis.w = y_axis.w = z_axis.w = 0.0;
    x_axis = normalize(x_axis);
    y_axis = normalize(y_axis);
    z_axis = normalize(z_axis);

    tci = mat4(
        vec4(x_axis.x, y_axis.x, z_axis.x, 0),
        vec4(x_axis.y, y_axis.y, z_axis.y, 0),
        vec4(x_axis.z, y_axis.z, z_axis.z, 0),
        vec4(0, 0, 0, 1)
    );

    tc = transpose(tci);

    tci = translateMat(position)* tci;
    tc = tc * translateMat(-position);


}


void Camera::rotateXAroundAt(float theta) {
    vec4 atc = tc * at; //at vector in camera coordinats;
    mat4 t1 = translateMat(atc);
    mat4 r = rotateXMat(theta);
    mat4 t2 = translateMat(-atc);
    tc = t1 * r * t2 * tc;

    mat4 ri = rotateXMat(-theta);
    tci = tci * t1 * ri * t2;

    position = tci * vec4(0, 0, 0, 1);//updating new position in world coordinates
}

void Camera::rotateYAroundAt(float theta) {
    vec4 atc = tc * at; //at vector in camera coordinats;
    mat4 t1 = translateMat(atc);
    mat4 r = rotateYMat(theta);
    mat4 t2 = translateMat(-atc);
    tc = t1 * r * t2 * tc;

    mat4 ri = rotateYMat(-theta);
    tci = tci * t1 * ri * t2;

    position = tci * vec4(0, 0, 0, 1);//updating new position in world coordinates
}

void Camera::rotateZAroundAt(float theta) {
    vec4 atc = tc * at; //at vector in camera coordinats;
    mat4 t1 = translateMat(atc);
    mat4 r = rotateZMat(theta);
    mat4 t2 = translateMat(-atc);
    tc = t1 * r * t2 * tc;

    mat4 ri = rotateYMat(-theta);
    tci = tci * t1 * ri * t2;

    position = tci * vec4(0, 0, 0, 1);//updating new position in world coordinates
}

void Camera::translateC(vec4 v) {
    mat4 t = translateMat(-v);
    mat4 ti = translateMat(v);

    vec4 atc = tc * at; //at point in camera coordinates, this should not change after translate

    tc = t * tc;
    tci = tci * ti;

    at = tci* atc;
    position = vec4(tci[0][3], tci[1][3], tci[2][3], 1); //tci* (0,0,0,1)

}



void Camera::lookAt() {
    vec4 z_axis = normalize(position - at);
    vec4 x_axis = normalize(cross(up, z_axis));
    vec4 y_axis = normalize(cross(z_axis, x_axis));
    vec4 t = vec4(0.0, 0.0, 0.0, 1.0);
    setTransformation(x_axis, y_axis, z_axis, position);
}

void Camera::lookAt(const vec4 at, vec4 up) {
    this->at = at;
    this->up = up;
    lookAt();
}

void Camera::zoom(float aspect) {

    vec4 v = vec4(0, 0, ((tc*at).z)*(aspect-1), 0);


    mat4 t = translateMat(-v);
    mat4 ti = translateMat(v);


    tc = t * tc;
    tci = tci * ti;

    position = vec4(tci[0][3], tci[1][3], tci[2][3], 1); //tci* (0,0,0,1)

}

//==========


//===Projections===

void Camera::ortho(const float left, const float right,
    const float bottom, const float top,
    const float z_near, const float z_far) {

    this->left = left;
    this->right = right;
    this->bottom = bottom;
    this->top = top;
    this->z_near = z_near;
    this->z_far = z_far;

    projection = p_ortho;

    mat4 t = translateMat(-(right + left) / 2, -(bottom + top) / 2, -(z_near + z_far) / 2);
    mat4 s = scaleMat(2 / (right - left), 2 / (top - bottom), 2 / -(z_near - z_far));
    tp = s * t;
}


void Camera::frustum(const float left, const float right,
    const float bottom, const float top,
    const float z_near, const float z_far) {

    this->left = left;
    this->right = right;
    this->bottom = bottom;
    this->top = top;
    this->z_near = z_near;
    this->z_far = z_far;

    projection = p_persp;

    mat4 h;
    h[0][2] = (left + right) / 2;
    h[1][2] = (top + bottom) / 2;
    mat4 s = scaleMat((-2 * z_near) / (right - left), -(2 * z_near) / (top - bottom), 1.0);
    mat4 n;
    n[2][2] = (z_near + z_far) / (z_near - z_far);
    n[2][3] = (2 * z_near * z_far) / (z_far-z_near);
    n[3][2] = -1;
    n[3][3] = 0;
    tp = n * s * h;
}

void Camera::perspective(const float fovy, const float aspect,
    const float z_near, const float z_far) {

    float r_angle = (M_PI / 180.0) * fovy;
    float h = 2 * (-z_near) * std::tan(r_angle / 2);
    float w = h * aspect;
    frustum(-w/2, w/2, -h/2, h/2, z_near, z_far);
}

void Camera::toggleProjection() {
    projection = proj_type(1 - projection);
    if (projection == p_ortho) {
        ortho(left, right, bottom, top, z_near, z_far);
    }
    else {
        frustum(left, right, bottom, top, z_near, z_far);
    }
}
//===========


//===Getters===

mat4 Camera::getTransform() { return tc; }

mat4 Camera::getProjection() { return tp; }

vec4 Camera::getPosition() { return position; }
vec4 Camera::getAt() { return at; }
vec4 Camera::getUp() { return up; }

//bool Camera::getIsActive() { return is_active; }

//void Camera::setIsActive(bool new_is_active) { is_active = new_is_active; }
//==========