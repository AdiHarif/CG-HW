#pragma once
#include <vector>
#include "CG_skel_w_MFC.h"
#include "vec.h"
#include "mat.h"
#include "GL/glew.h"
#include "Color.h"


#define M_OUT_BUFFER_SIZE 3*m_width*m_height

#define DEFAULT_WIDTH 512
#define DEFAULT_HEIGHT 512

using namespace std;
class Renderer
{
	float *m_outBuffer; // 3*width*height
	float *m_zbuffer; // width*height
	int m_width, m_height;

	mat4 viewport;


	//===Buffer Functions===
	void CreateBuffers();
	void CreateLocalBuffer(); //unimplemented
	//==========


	//===Transformations Calcs
	void calcViewport();
	//==========


	//===Inner Drawing Functions===
	void drawLineModerate(vec4 v0, vec4 v1, Color c);
	void drawLineSteep(vec4 v0, vec4 v1, Color c);
	//==========


	//////////////////////////////
	// openGL stuff. Don't touch.
	GLuint gScreenTex;
	GLuint gScreenVtc;
	void CreateOpenGLBuffer();
	void InitOpenGLRendering();
	//////////////////////////////

public:
	Renderer(int width=DEFAULT_WIDTH, int height=DEFAULT_HEIGHT);
	~Renderer(void);
	void init(); //unimplemented

	//===Buffer Interface
	void swapBuffers();
	void clearColorBuffer();
	void clearDepthBuffer(); //unimplemented

	void setSize(int width, int height);
	//==========

	//===Drawing Interface===
	void rasterizePoint(vec4 v, Color c);
	void rasterizeLine(vec4 v1, vec4 v2, Color c);

	void drawPoints(vector<vec4>& points, Color c);
	void drawTriangles(vector<vec4>& vertex_positions, Color c);
	void drawVertexNormals(vector<vec4>& vertices, vector<vec4>& vertex_normals, Color c);

	void SetDemoBuffer();
	//==========

	//===Getters===
	mat4 getViewport();
	//==========

	//===Transformation Setters===
	void setCameraTransform(const mat4& cTransform); //unimplemented
	void setProjection(const mat4& projection); //unimplemented
	void setObjectMatrices(const mat4& oTransform, const mat3& nTransform); //unimplemented
	//==========
};
