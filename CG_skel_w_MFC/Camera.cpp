#include "StdAfx.h"
#include "Camera.h"


//===Inner Calculations===
mat4 Camera::calcTcInverse() {
    return mat4(
        vec4(tc[0][0], tc[1][0], tc[2][0], -tc[0][3]),
        vec4(tc[0][1], tc[1][1], tc[2][1], -tc[1][3]),
        vec4(tc[0][2], tc[1][2], tc[2][2], -tc[2][3]),
        vec4(0, 0, 0, 1)
    );
}
//==========


//===C'tor===

Camera::Camera(vec4 position) {
    this->position = position;
    lookAt(vec4(0,0,0,1));
    ortho(-3.0, 3.0, -3.0, 3.0, -5.0, -15.0);
}

//==========


//===Transformations Interface===


void Camera::setTransformation(vec4 x_axis, vec4 y_axis, vec4 z_axis, vec4 position) {
    /*x_axis.w = y_axis.w = z_axis.w = 0.0;
    x_axis = normalize(x_axis);
    y_axis = normalize(y_axis);
    z_axis = normalize(z_axis);
    tc = mat4(x_axis, y_axis, z_axis, vec4(0, 0, 0, 1));
    tc = translateMat(-position) * tc;

    tci = mat4(
        vec4(x_axis.x, y_axis.x, z_axis.x, position.x),
        vec4(x_axis.y, y_axis.y, z_axis.y, position.y),
        vec4(x_axis.z, y_axis.z, z_axis.z, position.z),
        vec4(0,0,0,1)
    );*/

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


    std::cout << "setTransform: (before translate)" << std::endl << (tc * tci) << std::endl << std::endl;

    tci = translateMat(position)* tci;
    tc = tc * translateMat(-position);

    std::cout << "setTransform:" << std::endl << (tc * tci) << std::endl << std::endl;

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
    std::cout << "rotateYAroundAt:" << std::endl << (tc * tci) << std::endl << std::endl;
    std::cout << "rotateYAroundAt: (position)" << std::endl << position << std::endl << std::endl;
}

void translate(vec4 v) {

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

    mat4 t = translateMat(-(right + left) / 2, -(bottom + top) / 2, (z_near + z_far) / 2);
    mat4 s = scaleMat(2 / (right - left), 2 / (top - bottom), 2 / (z_near - z_far));
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
    mat4 s = scaleMat(-(2 * z_near) / (right - left), -(2 * z_near) / (top - bottom), 1.0);
    mat4 n;
    n[2][2] = (z_near + z_far) / (z_near - z_far);
    n[2][3] = (2 * z_near * z_far) / (z_near - z_far);
    n[3][2] = -1;
    n[3][3] = 0;
    tp = n * s * h;
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

//bool Camera::getIsActive() { return is_active; }

//void Camera::setIsActive(bool new_is_active) { is_active = new_is_active; }
//==========