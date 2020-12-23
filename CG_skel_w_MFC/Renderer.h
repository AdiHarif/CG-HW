#pragma once
#include <vector>
#include "CG_skel_w_MFC.h"
#include "vec.h"
#include "mat.h"
#include "GL/glew.h"
#include "Color.h"
#include "Face.h"
#include "Light.h"

#define M_OUT_BUFFER_SIZE 3*m_width*m_height
#define M_ZBUFFER_SIZE m_width*m_height

#define DEFAULT_WIDTH 512
#define DEFAULT_HEIGHT 512

using namespace std;

struct Pixel {
	int x;
	int y;
	float z;

	Pixel(int x, int y, float z) : x(x), y(y), z(z) {}

	Pixel(const Pixel& p) {
		x = p.x;
		y = p.y;
		z = p.z;
	}

};

struct Line {
	Pixel start;
	Pixel end;

	Line(Pixel p1, Pixel p2) : start(p1), end(p2) {	}
	Line(const Line& l) : start(l.start), end(l.end) {}

	//int findX(int y);
};


struct Triangle {
	vector<Line> lines;

	Triangle(vector<Line> lines) : lines(lines) {}
	Triangle(Pixel p0, Pixel p1, Pixel p2);
	Triangle(const Triangle& t) : lines(t.lines) {}

	int findMaxY();
	int findMinY();
};

 typedef vec4 Normal;

//struct Normal {
//	Vertex vertex;
//	vec4 direction;
//
//	Normal(Vertex vertex, vec4 direction) : vertex(vertex), direction(direction) {}
//};


class Renderer
{
	float *m_outBuffer; // 3*width*height
	float *m_zbuffer; // width*height
	int m_width, m_height;

	mat4 tw;
	mat4 tc;
	mat4 tp;

	bool f_anti_aliasing;
	vector<ParallelSource*>* parallel_sources = NULL;
	vector<PointSource*>* point_sources = NULL;

	Color* scene_ambient_light_color;
	//GLfloat* scene_ambient_light_intensity;


	//===Buffer Functions===
	void createBuffers();
	//void CreateLocalBuffer(); //unimplemented
	//==========


	//===Inner Drawing Functions===
	void drawPixel(Pixel p, Color c);
	void drawLine(Line l, Color c, vector<Pixel>* pixels_drawn = NULL);
	void drawLineModerate(Line l, Color c, vector<Pixel>* pixels_drawn = NULL);
	void drawLineSteep(Line l, Color c, vector<Pixel>* pixels_drawn = NULL);
	void drawTriangleSolid(Triangle t, Color c);
	//==========


	//===Inner Calculations===
	bool isPixelLegal(Pixel p);
	bool isLineLegal(Line l);
	Pixel viewPort(Vertex v);

	//vector<Pixel> transformVertices(vector<Vertex>& vertices, mat4 tm);
	//vector<Line> transformEdges(vector<vec4>& edges, mat4 tm); //Legacy, should delete this in the future.
	//vector<Line> transformEdges(vector<Edge>& edges, mat4 tm);
	//vector<Line> transformFaces(vector<Face>& faces, mat4 tm); 
	//vector<Line> transformVertexNormals(vector<Face>& faces, mat4 tm, mat4 ntm);
	//vector<Line> transformFaceNormals(vector<Face>& faces, mat4 tm, mat4 ntm);
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
	void clearBuffer();
	//void clearColorBuffer();
	void colorBackground(Color color);
	//void clearDepthBuffer(); //unimplemented

	void setSize(int width, int height);
	//==========

	//===Drawing Interface===
	//void drawPoints(vector<Vertex>& points, mat4 tm, Color c);
	//void drawLines(vector<vec4>& points, mat4 tm, Color c); //Legacy, should delete in the future (felt legacy, might delete later)
	//void drawLines(vector<Edge>& edges, mat4 tm, Color c);
	//void drawTriangles(vector<Face>& faces, mat4 tm, Color c);
	//void drawVertices(vector<Vertex>& vertices, mat4 tm, Color c);
	//void drawEdges(vector<Edge>& edges, mat4 tm, Color c);
	//void drawEdges(vector<Face>& faces, mat4 tm, Color c);
	//void drawVertexNormals(vector<Face>& faces, mat4 tm, mat4 ntm, Color c);
	//void drawFacesNormals(vector<Face>& faces, mat4 tm, mat4 ntm, Color c);
	void drawCamera(vec4 pos, Color c);

	void SetDemoBuffer();
	//==========


	////===Transformation Setters===
	void setCameraTransform(const mat4& tc);
	void setProjection(const mat4& tc);
	void setWorldTransform(const mat4& tw);
	////==========

	////===Lights Setters===
	void setParallelSources(vector<ParallelSource*>* parallel_sources);
	void setPointSources(vector<PointSource*>* point_sources);
	//void setAmbientConstants(Color* color, GLfloat* intensity);
	void setAmbientColor(Color* color);
	////==========


	void drawModel(MeshModel& model);

	void drawOrigin(Color c);

	//void drawTriangle();

	void toggleAntiAliasing();

	float* createAntiAliasedBuffer();
	////===Lighting Calculations===
	Color calculateAmbientColor(MeshModel& m);
	Color calculateDiffuse(Face f);

	////==========

};
