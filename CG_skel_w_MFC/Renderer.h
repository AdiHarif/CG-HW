#pragma once
#include <vector>
#include "CG_skel_w_MFC.h"
#include "vec.h"
#include "mat.h"
#include "GL/glew.h"
#include "Color.h"
#include "Face.h"
#include "Light.h"
#include "Camera.h"

#define M_OUT_BUFFER_SIZE 3*m_width*m_height
#define M_ZBUFFER_SIZE m_width*m_height

#define DEFAULT_WIDTH 512
#define DEFAULT_HEIGHT 512

using namespace std;

struct Pixel {
	int x;
	int y;
	float z;

	Color color;

	Pixel(int x, int y, float z, Color color = WHITE) : x(x), y(y), z(z), color(color){}

	Pixel(const Pixel& p) {
		x = p.x;
		y = p.y;
		z = p.z;
		color = p.color;
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

class Renderer
{
	float *m_outBuffer; // 3*width*height
	float *m_zbuffer; // width*height
	int m_width, m_height;

	//mat4 tw;
	/*mat4 tc;
	mat4 tp;*/

	Camera* camera;
	enum Shading { FLAT = 0, GOURAUD = 1, PHONG = 2 } shading_method = FLAT;

	vec4 active_camera_pos;

	bool f_anti_aliasing;
	bool f_fog;
	bool f_axes;

	struct Fog {
		Color c;
		float max_density;
	} fog;

	vector<ParallelSource>* parallel_sources;
	vector<PointSource>* point_sources;
	Color* scene_ambient_light_color;
	//GLfloat* scene_ambient_light_intensity;


	//===Buffer Functions===
	void createBuffers();
	//==========


	//===Inner Drawing Functions===
	void drawPixel(Pixel p);
	void drawLine(Line l, vector<Pixel>* pixels_drawn = NULL);
	void drawLineModerate(Line l, vector<Pixel>* pixels_drawn = NULL);
	void drawLineSteep(Line l, vector<Pixel>* pixels_drawn = NULL);
	void drawTriangleFlat(Triangle t, Color color);
	void drawTriangleGouraud(Triangle t, Color ambient_color, MeshModel* m, Face& f, vector<Vertex> wr_vertices, vector<Normal> normals);
	//==========


	//===Inner Calculations===
	bool isPixelLegal(Pixel p);
	bool isLineLegal(Line l);
	Pixel viewPort(Vertex v);
	//==========



	//////////////////////////////
	// openGL stuff. Don't touch.
	GLuint gScreenTex;
	GLuint gScreenVtc;
	void CreateOpenGLBuffer();
	void InitOpenGLRendering();
	//////////////////////////////

	friend class Scene;

public:
	Renderer(int width=DEFAULT_WIDTH, int height=DEFAULT_HEIGHT);
	~Renderer(void);
	void init(); //unimplemented

	//===Buffer Interface
	void swapBuffers();
	void clearBuffer();
	void colorBackground(Color color);
	void setSize(int width, int height);
	//==========

	void SetDemoBuffer();
	//==========


	////===Transformation/Camera Setters===
	/*void setCameraTransform(const mat4& tc);
	void setProjection(const mat4& tc);*/
	//void setWorldTransform(const mat4& tw);

	void setCamera(Camera* camera);

	//void setLightSources(vector<PointSource> points, vector<ParallelSource> parallels);
	void setActiveCameraPosition(vec4 pos);
	////===Lights Setters===
	void setAmbientColor(Color* color);
	////==========


	void drawModel(MeshModel& model);
	void drawCamera(vec4 pos, Color c);
	void drawOrigin(Color c);
	void drawLight(PointSource point_s);

	void toggleAntiAliasing();
	float* createAntiAliasedBuffer();

	////===Lighting Calculations===
	//Color calculateAmbientColor(MeshModel& m);
	Color calculateAmbientColor(Face& f);
	Color calculateDiffuseColor(Face& f, Vertex point, Normal noraml);
	Color calculateSpecularColor(MeshModel& m, Face& f, Vertex point, Normal normal, vec4 dir_to_camera);
	////==========

	void toggleFog();
	void applyFog();

	void drawAxes();

	void applyBlur(float* buffer);
	void applyBloom(float bloom_threshold);

	void toggleShading();
};
