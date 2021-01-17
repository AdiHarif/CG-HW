 #pragma once
#include "scene.h"
#include "vec.h"
#include "mat.h"
#include <string>
#include "Color.h"

#define BUFFERS_COUNT 2

using namespace std;

typedef struct s_draw_pref {
	enum PolyMode { VERTICES_ONLY = 0, EDGES_ONLY = 1, FILLED = 2} poly_mode = EDGES_ONLY;
	bool f_draw_bb = false;
	bool f_draw_vertex_normals = false;
	bool f_draw_faces_normals = false;
} DrawPref;


typedef enum {
	BT_VERTICES = 0,
	BT_NORMALS = 1
} BufferType;

class MeshModel : public Model
{
protected :

	/*vector<Face> faces;
	vector<Vertex> vertices;
	vector<Normal> vertex_normals;
	vector<Normal> face_normals;*/

	//vector<Edge> bb_edges;

	mat4 tm;
	mat4 tw;
	mat4 ntm;
	mat4 ntw;

	Color mesh_color;
	Color vertex_normals_color;
	Color face_normals_color;
	Color bb_color;

	DrawPref draw_pref;

	GLuint vao;

	GLuint vbos[BUFFERS_COUNT];

	int faces_count;

public:
	explicit MeshModel(string fileName = "");
	~MeshModel(void);
	void loadFile(string fileName);
	void initBoundingBox(vec4 min, vec4 max);

	void translate(vec4, bool f_world_frame = true);
	void scale(vec3, bool f_world_frame = false);
	void rotateX(GLfloat, bool f_world_frame = false);
	void rotateY(GLfloat, bool f_world_frame = false);
	void rotateZ(GLfloat, bool f_world_frame = false);

	vec4 getPosition();
	//vector<Face>* getFaces();
	
	void togglePolyMode();
	void toggleBB();
	void toggleVertexNormals();
	void toggleFaceNormals();

	void activate();
	void deactivate();

	void draw(mat4 tpc, GLuint program);

	friend class Scene;
};
