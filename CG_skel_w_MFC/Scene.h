	#pragma once

#include "gl/glew.h"
#include <vector>
#include <string>
#include "Renderer.h"
#include "Color.h"


using namespace std;

class Model {
protected:
	virtual ~Model() {}
	//void virtual draw()=0;
};


class Light {

};

class Camera {
	mat4 cTransform;
	mat4 projection;

	vec4 position;
	
	vec4 calcUp(vec4 at);

public:
	Camera(vec4 position);

	void setTransformation(const mat4& transform);
	void lookAt(const vec4& at);
	void ortho( const float left, const float right,
		const float bottom, const float top,
		const float z_near, const float z_far );
	void frustum( const float left, const float right,
		const float bottom, const float top,
		const float z_near, const float z_far );
	mat4 perspective( const float fovy, const float aspect,
		const float z_near, const float z_far);

	mat4 getTransform();
	mat4 getProjection();
};

class Scene {

	vector<Model*> models;
	vector<Light*> lights;
	vector<Camera*> cameras;
	Renderer *m_renderer;

public:
	Scene() {};
	Scene(Renderer *renderer);
	void loadOBJModel(string fileName);
	void draw();
	void drawDemo();
	void scale(double scale_factor);
	void rotateX(double theta);
	void rotateY(double theta);
	void rotateZ(double theta);
	void translate(vec4 vec);
	void toggleVertices();
	void toggleEdges();
	void toggleBB();
	void toggleVertexNormals();
	void toggleFaceNormals();
	
	void addCamera(Camera* camera);


	int activeModel;
	int activeLight;
	int activeCamera;
};