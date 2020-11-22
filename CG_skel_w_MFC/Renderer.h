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

typedef struct Pixel {
	int x;
	int y;
	float z;

	Pixel(int x, int y, float z = 0) : x(x), y(y), z(z) {}

	Pixel(const Pixel& p) {
		x = p.x;
		y = p.y;
		z = p.z;
	}

};

typedef struct Line {
	Pixel start;
	Pixel end;

	Line(Pixel p1, Pixel p2) : start(p1), end(p2) {	}

	Line(const Line& l) : start(l.start), end(l.end) {}
};

typedef vec4 Vertex;

typedef struct Edge {
	Vertex start;
	Vertex end;

	Edge(Vertex start, Vertex end) : start(start), end(end) {}
};

typedef struct Normal {
	Vertex vertex;
	vec4 direction;

	Normal(Vertex vertex, vec4 direction) : vertex(vertex), direction(direction) {}
};



class Renderer
{
	float *m_outBuffer; // 3*width*height
	float *m_zbuffer; // width*height
	int m_width, m_height;

	mat4 tw;
	mat4 tc;
	mat4 tp;



	//===Buffer Functions===
	void CreateBuffers();
	void CreateLocalBuffer(); //unimplemented
	//==========


	//===Inner Drawing Functions===
	void rasterizePoint(Pixel p, Color c);
	void rasterizeLine(Line l, Color c);
	void drawLineModerate(Line l, Color c);
	void drawLineSteep(Line l, Color c);
	//==========


	//===Inner Calculations===
	bool isPixelLegal(Pixel p);
	bool isLineLegal(Line l);
	Pixel viewPort(Vertex v);

	vector<Pixel> transformVertices(vector<Vertex>& vertices, mat4 tm);
	vector<Line> transformEdges(vector<vec4>& edges, mat4 tm); //Legacy, should delete this in the future.
	vector<Line> transformEdges(vector<Edge>& edges, mat4 tm);
	vector<Line> transformFaces(vector<vec4>& faces, mat4 tm); //TODO change this to work with face struct
	vector<Line> transformNormals(vector<Normal>& normals, mat4 tm, mat4 ntm);
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
	void colorBackground(Color color);
	void clearDepthBuffer(); //unimplemented

	void setSize(int width, int height);
	//==========

	//===Drawing Interface===
	void drawPoints(vector<Vertex>& points, mat4 tm, Color c);
	void drawLines(vector<vec4>& points, mat4 tm, Color c); //Legacy, shoud delete in the future (felt legacy, might delete later)
	void drawLines(vector<Edge>& edges, mat4 tm, Color c);
	void drawTriangles(vector<vec4>& vertex_positions, mat4 tm, Color c);
	void drawVertexNormals(vector<Normal>& normals, mat4 tm, mat4 ntm, Color c);
	void drawFacesNormals(vector<Normal>& normals, mat4 tm, mat4 ntm, Color c);
	void drawCamera(vec4 pos, vec4 at, vec4 up, Color c);

	void SetDemoBuffer();
	//==========


	//===Transformation Setters===
	void setCameraTransform(const mat4& tc);
	void setProjection(const mat4& tc);
	void setWorldTransform(const mat4& tw);
	//==========
};
