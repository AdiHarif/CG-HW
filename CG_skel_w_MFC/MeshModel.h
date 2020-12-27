#pragma once
#include "scene.h"
#include "vec.h"
#include "mat.h"
#include <string>
#include "Renderer.h"
#include "Color.h"

using namespace std;

typedef struct s_draw_pref {
	enum PolyMode { VERTICES_ONLY = 0, EDGES_ONLY = 1, FILLED = 2} poly_mode = EDGES_ONLY;
	bool f_draw_bb = false;
	bool f_draw_vertex_normals = false;
	bool f_draw_faces_normals = false;
} DrawPref;


class MeshModel : public Model
{
protected :
	MeshModel() {}

	vector<Face> faces;
	vector<Vertex> vertices;
	vector<Normal> vertex_normals;
	vector<Normal> face_normals;

	vector<Edge> bb_edges;

	vec4 position;
	mat4 tm;
	mat4 tw;
	mat4 ntm;
	mat4 ntw;

	Color mesh_color;
	Color vertex_normals_color;
	Color face_normals_color;
	Color bb_color;

	DrawPref draw_pref;


public:
	MeshModel(string fileName);
	~MeshModel(void);
	void loadFile(string fileName);
	void initBoundingBox(vec4 min, vec4 max);

	void translate(vec4);
	void scale(vec3);
	void rotateX(GLfloat);
	void rotateY(GLfloat);
	void rotateZ(GLfloat);

	mat4 getWorldTransform();
	vec4 getPosition();
	
	void togglePolyMode();
	void toggleBB();
	void toggleVertexNormals();
	void toggleFaceNormals();

	void activate();
	void deactivate();

	friend class Renderer;
};
