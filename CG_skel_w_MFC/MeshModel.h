#pragma once
#include "scene.h"
#include "vec.h"
#include "mat.h"
#include <string>
#include "Renderer.h"
#include "Color.h"

using namespace std;

typedef struct s_draw_pref {
	bool f_draw_vertices = true;
	bool f_draw_edges = true;
	bool f_draw_bb = false;
	bool f_draw_vertex_normals = false;
	bool f_draw_faces_normals = false;
} DrawPref;

class MeshModel : public Model
{
protected :
	MeshModel() {}

	//vector<FaceIdcs> faces;
	vector<vec4> vertices;
	vector<vec4> vertex_positions;
	vector<vec4> vertex_normals;
	vector<vec4> bounding_box_vertices;
	//add more attributes
	vec4 position;
	mat4 _world_transform;
	mat3 _normal_transform;

	Color mesh_color;
	Color vertex_normals_color;
	Color faces_normals_color;
	Color bb_color;

	DrawPref draw_pref;

	


public:
	void draw(mat4 tcw, Renderer* renderer);
	void drawVertices(mat4 tcw, Renderer* renderer);
	void drawEdges(mat4 tcw, Renderer* renderer);
	void drawVertexNormals(mat4 tcw, Renderer* renderer);
	void drawBoundingBox(mat4 tcw, Renderer* renderer);
	void initBoundingBox(vec4 min, vec4 max);

	MeshModel(string fileName);
	~MeshModel(void);
	void loadFile(string fileName);
	void translate(vec4);
	void scale(vec3);
	void rotateX(GLfloat);
	void rotateY(GLfloat);
	void rotateZ(GLfloat);
	vector<vec4> getVertices();
	vector<vec4> getVertexPositions();
	mat4 getWorldTransform();
	vec4 getPosition();
	
	void toggleVertices();
	void toggleEdges();
	void toggleBB();
	void toggleVertexNormals();
	void toggleFaceNormals();
};
