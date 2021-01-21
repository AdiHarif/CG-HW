#pragma once

#include "Color.h"

class Model {
protected:
	Color emit_color = { 0.0, 0.0, 0.1 };
	Color ambient_color = PURPLE;
	Color diffuse_color = WHITE;
	Color specular_color = WHITE;

	//GLfloat specular_reflection;
	//GLfloat diffuse_reflection;
	//GLfloat ambient_reflection = 1;
	GLfloat shininess = 0.1;

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

	virtual void draw() = 0;

	friend class Scene;
};