#pragma once

#include "Color.h"

#define WAVE_THRESHOLD_STEP 0.01
#define VERTEX_ANIMATION_X_STEP 0.02
#define VERTEX_ANIMATION_X_MIN 0
#define VERTEX_ANIMATION_X_MAX 1
#define NUM_OF_VERTEX_ANIMATION_DIRECTIONS 2

typedef enum {
	VA_IN,
	VA_OUT
}VertexAnimationDirection;

class Model {
protected:
	Color emit_color = { 0.0, 0.0, 0.1 };
	Color ambient_color = PURPLE;
	Color diffuse_color = WHITE;
	Color specular_color = WHITE;
	GLfloat shininess = 0.1;

	Color hsv_color = { 0.0, 0.8, 0.8 };

	Color wave_color_0 = Color::getRandomColor();
	Color wave_color_1 = Color::getRandomColor();
	float wave_threshold = -1;

	float vertex_animation_x = 1;
	float vertex_animation_t = 0;
	VertexAnimationDirection current_vertex_animation_dir = VA_IN;

	virtual ~Model() {}
	//void virtual draw()=0;

public:
	Color getAmbientColor() { return ambient_color; }
	void setAmbientColor(Color c) { ambient_color = c; }

	Color getDiffuseColor() { return diffuse_color; }
	void setDiffuseColor(Color c) { diffuse_color = c; }

	Color getSpecularColor() { return specular_color; }
	void setSpecularColor(Color c) { specular_color = c; }

	Color getEmitColor() { return emit_color; }
	void setEmitColor(Color c) { emit_color = c; }

	GLfloat getShininess() { return shininess; }
	void setShininess(GLfloat s) { shininess = s; }

	GLfloat easeInCubic(GLfloat x) { return -x * x * x; }
	GLfloat easeOutCubic(GLfloat x) { return -2*(1 - pow(1 - x, 3)); }



	virtual void draw() = 0;

	friend class Scene;
};