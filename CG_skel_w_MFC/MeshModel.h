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


struct FaceIdcs
{
	int v[4];
	int vn[4];
	int vt[4];

	FaceIdcs()
	{
		for (int i = 0; i < 4; i++)
			v[i] = vn[i] = vt[i] = 0;
	}

	FaceIdcs(vec4 v0) {
		v[0] = v0.x;
		v[1] = v0.y;
		v[2] = v0.z;
		v[3] = 0;
		for (int i = 0; i < 4; i++)
			vn[i] = vt[i] = 0;
	}

	FaceIdcs(std::istream& aStream)
	{
		for (int i = 0; i < 4; i++)
			v[i] = vn[i] = vt[i] = 0;

		char c;
		for (int i = 0; i < 3; i++)
		{
			aStream >> std::ws >> v[i] >> std::ws;
			if (aStream.peek() != '/')
				continue;
			aStream >> c >> std::ws;
			if (aStream.peek() == '/')
			{
				aStream >> c >> std::ws >> vn[i];
				continue;
			}
			else
				aStream >> vt[i];
			if (aStream.peek() != '/')
				continue;
			aStream >> c >> vn[i];
		}
	}
};

class MeshModel : public Model
{
protected :
	MeshModel() {}

	vector<FaceIdcs> faces;
	vector<vec4> vertices;
	vector<vec4> vertex_positions;
	vector<vec4> vertex_normals;
	vector<vec4> bounding_box_vertices;
	//vector<int> vertex_normals_indexes;
	//vector<vec4> faces_normals;
	//vector<vec4> faces_normals_locations;
	vector<Normal> vertices_to_normals;
	vector<Normal> faces_to_normals;

	vec4 position;
	mat4 tm;
	mat4 ntm;
	mat3 _normal_transform;

	bool is_active;

	Color active_mesh_color;
	Color inactive_mesh_color;
	Color vertex_normals_color;
	Color faces_normals_color;
	Color bb_color;

	DrawPref draw_pref;

	


public:
	MeshModel(string fileName);
	~MeshModel(void);
	void loadFile(string fileName);

	void draw(Renderer* renderer);
	void drawVertices(Renderer* renderer);
	void drawEdges(Renderer* renderer);
	void drawVertexNormals(Renderer* renderer);
	void drawFacesNormals(Renderer* renderer);
	void drawBoundingBox(Renderer* renderer);
	void initBoundingBox(vec4 min, vec4 max);
	void computeFacesNormals();

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

	bool getIsActive();
	void setIsActive(bool new_is_active);
};
