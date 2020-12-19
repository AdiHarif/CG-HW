#include "StdAfx.h"
#include "Renderer.h"
#include "CG_skel_w_MFC.h"
#include "InitShader.h"
#include "GL\freeglut.h"
#include "MeshModel.h"

#define INDEX(width,x,y,c) (x+y*width)*3+c
#define Z_INDEX(width,x,y) (x+y*width)
#define LEFT_PIXEL 0
#define RIGHT_PIXEL 1
#define DRAW_BETWEEN_COLS 2

//===Line Functions===

//int Line::findX(int y) {
//	return (int)( ((end.x - start.x) * (y - start.y))/(end.y - start.y) + start.x + 0.5);
//}

//==========

//===Triangle Functions===

Triangle::Triangle(Pixel p0, Pixel p1, Pixel p2) {
	this->lines.push_back(Line(p0, p1));
	this->lines.push_back(Line(p1, p2));
	this->lines.push_back(Line(p2, p0));
}

int Triangle::findMaxY() {
	int tmp = max(lines[0].start.y, lines[1].start.y);
	return max(tmp, lines[2].start.y);
}

int Triangle::findMinY() {
	int tmp = min(lines[0].start.y, lines[1].start.y);
	return min(tmp, lines[2].start.y);
}

//==========


//===Buffer Functions===
void Renderer::createBuffers()
{
	CreateOpenGLBuffer(); //Do not remove this line.
	m_outBuffer = new float[3*m_width*m_height];
	m_zbuffer = new float[m_width * m_height];
}
//==========


//===Inner Drawing Functions===

void Renderer::drawPixel(Pixel p, Color c) {
	if ( (!isPixelLegal(p)) || p.z < m_zbuffer[Z_INDEX(m_width, p.x, p.y)]) {
		return;
	}
	m_outBuffer[INDEX(m_width, p.x, p.y, 0)] = c.r;
	m_outBuffer[INDEX(m_width, p.x, p.y, 1)] = c.g;
	m_outBuffer[INDEX(m_width, p.x, p.y, 2)] = c.b;
	m_zbuffer[Z_INDEX(m_width, p.x, p.y)] = p.z;
}

void Renderer::drawLine(Line l, Color c, vector<Pixel>* pixels_drawn) {
	Pixel p0 = l.start;
	Pixel p1 = l.end;
	if (p0.x == p1.x) {
		if (p0.y < p1.y) {
			drawLineSteep(Line(p0,p1), c, pixels_drawn);
		}
		else {
			drawLineSteep(Line(p1, p0), c, pixels_drawn);
		}
	}
	if (p0.x > p1.x) {// make sure that v0 is left of v1
		p0 = l.end;
		p1 = l.start;
	}

	if (std::abs(p1.y - p0.y) < std::abs(p1.x - p0.x)) {//-1 <= m <= 1
		drawLineModerate(Line(p0, p1), c, pixels_drawn);
	}
	else { //|m|>1
		drawLineSteep(Line(p0, p1), c, pixels_drawn);
	}
}


void Renderer::drawLineModerate(Line l, Color c, vector<Pixel>* pixels_drawn) {
	Pixel p0 = l.start;
	Pixel p1 = l.end;
	int dx = p1.x - p0.x;
	int dy = p1.y - p0.y;

	float dz = (p1.z - p0.z) / dx;
	float z = p0.z;


	int yi = 1;
	if (dy < 0) {
		yi = -1;
		dy = -dy;
	}
	int D = 2*dy - dx;
	int y = p0.y;
	for (int x = p0.x; x <= p1.x; x++) {
		Pixel p = Pixel(x, y, z);
		if (pixels_drawn != NULL) {
			pixels_drawn->push_back(p);
		}
		this->drawPixel(p, c);
		if (D > 0) {
			y += yi;
			D += 2*(dy-dx);
		}
		else {
			D += 2*dy;
		}
		z += dz;
	}
}

void Renderer::drawLineSteep(Line l, Color c, vector<Pixel>* pixels_drawn) {
	//flip x<->y
	Pixel p0 = { l.start.y, l.start.x, l.start.z};
	Pixel p1 = { l.end.y, l.end.x, l.end.z};
	//make sure p0 is left of p1
	if (p0.x > p1.x) {
		Pixel tmp = p0;
		p0 = p1;
		p1 = tmp;
	}
	int dx = p1.x - p0.x;
	int dy = p1.y - p0.y;

	float dz = (p1.z - p0.z) / dx;
	float z = p0.z;

	int yi = 1;
	if (dy < 0) {
		yi = -1;
		dy = -dy;
	}
	int D = 2 * dy - dx;
	int y = p0.y;
	for (int x = p0.x; x <= p1.x; x++) {
		Pixel p = Pixel(y, x, z);
		if (pixels_drawn != NULL) {
			pixels_drawn->push_back(p);
		}
		this->drawPixel(p, c);
		if (D > 0) {
			y += yi;
			D += 2 * (dy - dx);
		}
		else {
			D += 2 * dy;
		}

		z += dz;
	}
}

void Renderer::drawTriangleSolid(Triangle t, Color c) {
	int max_y = t.findMaxY();
	int min_y = t.findMinY();
	int rows = max_y - min_y + 1;
	//auto draw_between = new int[rows][DRAW_BETWEEN_COLS]; // creating a [rows][2] array

	//// allocate 2d array
	//int** draw_between = new int* [rows];
	//int draw_between[rows][DRAW_BETWEEN_COLS];
	//for (int i = 0; i < rows; i++) {
	//	draw_between[i] = { INT_MAX, INT_MIN };
	//}
	//// ---

	vector<Line> draw_between;
	for (int i = 0; i < rows; i++) {
		Pixel left = Pixel(INT_MIN, i + min_y, -1);// TODO: add z
		Pixel right = Pixel(INT_MAX, i + min_y, -1);// TODO: add z
		draw_between.push_back(Line(left, right));
	}

	for (Line l : t.lines) {
		vector<Pixel> pixels_drawn;
		drawLine(l, c, &pixels_drawn);
		for (Pixel p : pixels_drawn) {
			if (p.x > draw_between.at(p.y - min_y).start.x) {
				draw_between.at(p.y - min_y).start.x = p.x;
				draw_between.at(p.y - min_y).start.z = p.z;
			}
			if (p.x < draw_between.at(p.y - min_y).end.x) {
				draw_between.at(p.y - min_y).end.x = p.x;
				draw_between.at(p.y - min_y).end.z = p.z;
			}
		}
	}

	for (int i = 0; i < rows; i++) {
		Line line_to_draw = Line(draw_between.at(i).start, draw_between.at(i).end);
		drawLine(line_to_draw, c);
	}

	//// delete 2d array
	//for (int i = 0; i < rows; ++i) {
	//	delete[] draw_between[i];
	//}
	//delete[] draw_between;
	//// ---
}

//==========


//===Inner Calculations===

bool Renderer::isPixelLegal(Pixel p) {
	return p.x >= 0 && p.x < m_width&& p.y >= 0 && p.y < m_height&& p.z>-1 && p.z < 1;
}

bool Renderer::isLineLegal(Line l) {
	return !(
		(l.start.x < 0 && l.end.x < 0) ||
		(l.start.x > m_width && l.end.x > m_width) ||
		(l.start.y < 0 && l.end.y < 0) ||
		(l.start.y > m_height && l.end.y > m_height)
		);
}

Pixel Renderer::viewPort(Vertex v) {
	v = v / v.w;
	Pixel p = {
		(int)std::round((m_width * (v.x + 1)) / 2),
		(int)std::round((m_height * (v.y + 1)) / 2),
		v.z
	};
	return p;
}

//vector<Pixel> Renderer::transformVertices(vector<Vertex>& vertices, mat4 tm){ //TODO: improve clipping
//	vector<Pixel> pixels;
//	mat4 t_tot = tp * tc * tw * tm;
//	for (vector<vec4>::iterator i = vertices.begin(); i != vertices.end(); i++) {
//		vec4 v = t_tot * (*i);
//		v = v/v.w;
//		if (v.z < -1 || v.z > 1) continue;
//		Pixel p = viewPort(v);
//		if (isPixelLegal(p)) {
//			pixels.push_back(p);
//		}
//	}
//	return pixels;
//}


//vector<Line> Renderer::transformEdges(vector<vec4>& edges, mat4 tm) { //TODO:delete
//	vector<Line> lines;
//	mat4 t_tot = tp * tc * tw * tm;
//	for (vector<vec4>::iterator i = edges.begin(); i != edges.end(); i += 2) {
//		vec4 v0 = t_tot * (*i);
//		v0 = v0 / v0.w;
//		vec4 v1 = t_tot * (*(i + 1));
//		v1 = v1 / v1.w;
//		if ((v0.z < -1 && v1.z < -1) || (v0.z > 1 && v1.z > 1)) continue;
//		/*if (v0.z > v1.z) {
//			vec4 tmp = v0;
//			v0 = v1;
//			v1 = tmp;
//		}
//		if (v0.z < -1) {
//			vec4 correction = (v1 - v0) * ( (-1-v0.z)/(v1.z-v0.z));
//			v0 = v0 + correction;
//		}
//		if (v1.z > 1) {
//			vec4 correction = (v0-v1) * ((1 - v1.z) / (v0.z - v1.z));
//			v0 = v0 + correction;
//		}*/
//
//
//		Line l = Line(viewPort(v0), viewPort(v1));
//		if (isLineLegal(l)) {
//			lines.push_back(l);
//		}
//	}
//	return lines;
//}

//vector<Line> Renderer::transformEdges(vector<Edge>& edges, mat4 tm) { //TODO: improve clipping
//	vector<Line> lines;
//	mat4 t_tot = tp * tc * tw * tm;
//	for (vector<Edge>::iterator i = edges.begin(); i != edges.end(); i++) {
//		vec4 v0 = t_tot * (i->start);
//		v0 = v0 / v0.w;
//		vec4 v1 = t_tot * (i->end);
//		v1 = v1 / v1.w;
//		if ((v0.z < -1 && v1.z < -1) || (v0.z > 1 && v1.z > 1)) continue;
//		/*if (v0.z > v1.z) {
//			vec4 tmp = v0;
//			v0 = v1;
//			v1 = tmp;
//		}
//		if (v0.z < -1) {
//			vec4 correction = (v1 - v0) * ((-1 - v0.z) / (v1.z - v0.z));
//			v0 = v0 + correction;
//		}
//		if (v1.z > 1) {
//			vec4 correction = (v0 - v1) * ((1 - v1.z) / (v0.z - v1.z));
//			v0 = v0 + correction;
//		}*/
//		Line l = Line(viewPort(v0),viewPort(v1));
//		if (isLineLegal(l)) {
//			lines.push_back(l);
//		}
//	}
//	return lines;
//}
//
//vector<Line> Renderer::transformFaces(vector<Face>& faces, mat4 tm) {
//	vector<Line> lines;
//	mat4 t_tot = tp * tc * tw * tm;
//	for (vector<Face>::iterator i = faces.begin(); i != faces.end(); i++) {
//		Face f = (*i);
//		for (int j = 0; j < 3; j++) {
//			vec4 v0 = t_tot * (*f.vertices[j]);
//			v0 = v0 / v0.w;
//			vec4 v1 = t_tot * (*f.vertices[(j + 1) % 3]);
//			v1 = v1 / v1.w;
//			if ((v0.z < -1 && v1.z < -1) || (v0.z > 1 && v1.z > 1)) continue;
//			/*if (v0.z > v1.z) {
//				vec4 tmp = v0;
//				v0 = v1;
//				v1 = tmp;
//			}
//			if (v0.z < -1) {
//				vec4 correction = (v1 - v0) * ((-1 - v0.z) / (v1.z - v0.z));
//				v0 = v0 + correction;
//			}
//			if (v1.z > 1) {
//				vec4 correction = (v0 - v1) * ((1 - v1.z) / (v0.z - v1.z));
//				v0 = v0 + correction;
//			}*/
//			Line l = Line(viewPort(v0),viewPort(v1));
//			if (isLineLegal(l)) {
//				lines.push_back(l);
//			}
//		}
//	}
//	return lines;
//}

//vector<Line> Renderer::transformVertexNormals(vector<Face>& faces, mat4 tm, mat4 ntm) {
//	vector<Line> lines;
//	mat4 tm_tot = tp * tc * tw * tm;
//	mat4 ntm_t1 = tw * ntm;
//	mat4 ntm_t2 = tp * tc;
//	for (int i = 0; i < faces.size(); i++) {
//		for (int j = 0; j < 3; j++) {
//			vec4 start = tm_tot * (*faces[i].vertices[j]);
//			start = start / start.w;
//			vec4 direction = ntm_t1 * (*faces[i].normals[j]);
//			direction.w = 0;
//			direction = normalize(direction);
//			direction.w = 1;
//			direction = ntm_t2 * direction;
//			direction = direction / direction.w;
//			vec4 end = start + direction;
//			end.w = 1;
//			if ((start.z < -1 && end.z < -1) || (start.z > 1 && end.z > 1)) continue;
//			Line l = Line(viewPort(start), viewPort(end));
//			if (isLineLegal(l)) {
//				lines.push_back(l);
//			}
//		}
//	}
//	return lines;
//}
//
//vector<Line> Renderer::transformFaceNormals(vector<Face>& faces, mat4 tm, mat4 ntm) {
//	vector<Line> lines;
//	mat4 tm_tot = tp * tc * tw * tm;
//	mat4 ntm_t1 = tw * ntm;
//	mat4 ntm_t2 = tp * tc;
//	for (int i = 0; i < faces.size(); i++) {
//		vec4 start = tm_tot * (faces[i].center);
//		start = start / start.w;
//		vec4 direction = ntm_t1 * (faces[i].center_normal);
//		direction.w = 0;
//		direction = normalize(direction);
//		direction.w = 1;
//		direction = ntm_t2 * direction;
//		direction = direction / direction.w;
//		vec4 end = start + direction;
//		end.w = 1;
//		if ((start.z < -1 && end.z < -1) || (start.z > 1 && end.z > 1)) continue;
//		Line l = Line(viewPort(start), viewPort(end));
//		if (isLineLegal(l)) {
//			lines.push_back(l);
//		}
//	}
//	return lines;
//}

//==========


/////////////////////////////////////////////////////
//OpenGL stuff. Don't touch.
void Renderer::InitOpenGLRendering()
{
	int a = glGetError();
	a = glGetError();
	glGenTextures(1, &gScreenTex);
	a = glGetError();
	glGenVertexArrays(1, &gScreenVtc);
	GLuint buffer;
	glBindVertexArray(gScreenVtc);
	glGenBuffers(1, &buffer);
	const GLfloat vtc[]={
		-1, -1,
		1, -1,
		-1, 1,
		-1, 1,
		1, -1,
		1, 1
	};
	const GLfloat tex[]={
		0,0,
		1,0,
		0,1,
		0,1,
		1,0,
		1,1};
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vtc)+sizeof(tex), NULL, GL_STATIC_DRAW);
	glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(vtc), vtc);
	glBufferSubData( GL_ARRAY_BUFFER, sizeof(vtc), sizeof(tex), tex);

	GLuint program = InitShader( "vshader.glsl", "fshader.glsl" );
	glUseProgram( program );
	GLint  vPosition = glGetAttribLocation( program, "vPosition" );

	glEnableVertexAttribArray( vPosition );
	glVertexAttribPointer( vPosition, 2, GL_FLOAT, GL_FALSE, 0,
		0 );

	GLint  vTexCoord = glGetAttribLocation( program, "vTexCoord" );
	glEnableVertexAttribArray( vTexCoord );
	glVertexAttribPointer( vTexCoord, 2, GL_FLOAT, GL_FALSE, 0,
		(GLvoid *) sizeof(vtc) );
	glProgramUniform1i( program, glGetUniformLocation(program, "texture"), 0 );
	a = glGetError();
}

void Renderer::CreateOpenGLBuffer()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gScreenTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, m_width, m_height, 0, GL_RGB, GL_FLOAT, NULL);
	glViewport(0, 0, m_width, m_height);
}
/////////////////////////////////////////////////////


//===C'tor\D'tor===
Renderer::Renderer(int width, int height) :m_width(width), m_height(height)
{
	InitOpenGLRendering();
	createBuffers();
}

Renderer::~Renderer(void)
{
	delete[] m_outBuffer;
	delete[] m_zbuffer;
}
//==========


//===Buffer Interface
void Renderer::swapBuffers()
{

	int a = glGetError();
	glActiveTexture(GL_TEXTURE0);
	a = glGetError();
	glBindTexture(GL_TEXTURE_2D, gScreenTex);
	a = glGetError();
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_width, m_height, GL_RGB, GL_FLOAT, m_outBuffer);
	glGenerateMipmap(GL_TEXTURE_2D);
	a = glGetError();

	glBindVertexArray(gScreenVtc);
	a = glGetError();
	glDrawArrays(GL_TRIANGLES, 0, 6);
	a = glGetError();
	glutSwapBuffers();
	a = glGetError();
}

void Renderer::clearBuffer() {
	for (int i = 0; i < M_OUT_BUFFER_SIZE; i++) {
		m_outBuffer[i] = 0.2;
	}
	for (int i = 0; i < M_ZBUFFER_SIZE; i++) {
		m_zbuffer[i] = -1;
	}
}

//void Renderer::clearColorBuffer() {
//	for (int i = 0; i < M_OUT_BUFFER_SIZE; i++) {
//		m_outBuffer[i] = 0.2;
//	}
//}
//
void Renderer::colorBackground(Color color) {
	for (int i = 0; i < M_OUT_BUFFER_SIZE; i+=3) {
		m_outBuffer[i] = color.r;
		m_outBuffer[i + 1] = color.g;
		m_outBuffer[i + 2] = color.b;
	}
}


void Renderer::setSize(int width, int height) {
	delete[] m_outBuffer;
	delete[] m_zbuffer;
	m_width = width;
	m_height = height;
	createBuffers();
}
//==========


//===Drawing Interface===
//void Renderer::drawVertices(vector<Vertex>& vertices, mat4 tm, Color c){
//	vector<Pixel> pixels = transformVertices(vertices, tm);
//	for (vector<Pixel>::iterator i = pixels.begin(); i != pixels.end(); i++) {
//		rasterizePoint(*i, c);
//	}
//}
//
//void Renderer::drawEdges(vector<Edge>& edges, mat4 tm, Color c) {
//	vector<Line> lines = transformEdges(edges, tm);
//	for (vector<Line>::iterator i = lines.begin(); i != lines.end(); i++) {
//		rasterizeLine(*i, c);
//	}
//}
//
//void Renderer::drawEdges(vector<Face>& faces, mat4 tm, Color c) {
//	vector<Line> lines = transformFaces(faces, tm);
//	for (vector<Line>::iterator i = lines.begin(); i != lines.end(); i++) {
//		rasterizeLine(*i, c);
//	}
//}

//void Renderer::drawTriangles(vector<Face>& faces, mat4 tm, Color c) {
//	vector<Line> lines = transformFaces(faces, tm);
//	for (vector<Line>::iterator i = lines.begin(); i != lines.end(); i++) {
//		rasterizeLine(*i, c);
//	}
//}

//void Renderer::drawVertexNormals(vector<Vertex>& points, mat4 tm, vector<vec4>& normals, mat4 ntm, vector<int>& vertex_normals_indexes, Color c) {
	//vector<vec4> normals_to_draw;
	/*for (vector<vec4>::iterator i = normals.begin(); i != normals.end(); i++) {
		*i = normalize(*i);
		*i *= 10;
	}
	vector<Pixel> vertices_pixels = transformVertices(points, tm);
	vector<Pixel> normals_pixels = transformVertices(normals, ntm);
	

	for (int i = 0; i < vertex_normals_indexes.size(); i+=2) {
		Pixel start = vertices_pixels[vertex_normals_indexes[i]];
		Pixel end = Pixel(0, 0); 
		end.x = vertices_pixels[vertex_normals_indexes[i]].x + normals[vertex_normals_indexes[i + 1]].x;
		end.y = vertices_pixels[vertex_normals_indexes[i]].y + normals[vertex_normals_indexes[i + 1]].y;

		Line l = Line(start, end);
		rasterizeLine(l, c);
	}
}*/

//void Renderer::drawVertexNormals(vector<Face>& faces, mat4 tm, mat4 ntm, Color c) {
//	vector<Line> lines = transformVertexNormals(faces, tm, ntm);
//	for (vector<Line>::iterator i = lines.begin(); i != lines.end(); i++) {
//		rasterizeLine(*i, c);
//	}
//}
//
//void Renderer::drawFacesNormals(vector<Face>& faces, mat4 tm, mat4 ntm, Color c) {
//	vector<Line> lines = transformFaceNormals(faces, tm, ntm);
//	for (vector<Line>::iterator i = lines.begin(); i != lines.end(); i++) {
//		rasterizeLine(*i, c);
//	}
//}
//
void Renderer::drawCamera(vec4 pos, Color c) {
	mat4 t_tot = tp * tc * tw;
	Pixel center = viewPort(t_tot * pos);
	for (int j = -2; j <= 2; j++) {
		drawPixel(Pixel(center.x+j, center.y, center.z), c);
		drawPixel(Pixel(center.x, center.y+j, center.z), c);
	}
}

void Renderer::SetDemoBuffer()
{
	//vertical line
	for(int i=0; i<m_width; i++)
	{
		m_outBuffer[INDEX(m_width,256,i,0)]=1;	m_outBuffer[INDEX(m_width,256,i,1)]=0;	m_outBuffer[INDEX(m_width,256,i,2)]=0;
	}
	//horizontal line
	for(int i=0; i<m_height; i++)
	{
		m_outBuffer[INDEX(m_width,i,256,0)]=1;	m_outBuffer[INDEX(m_width,i,256,1)]=0;	m_outBuffer[INDEX(m_width,i,256,2)]=1;
	}

	//diagonal line
	for (int i = 0; i < m_height; i++)
	{
		m_outBuffer[INDEX(m_width, m_height-i, i, 0)] = 1;	m_outBuffer[INDEX(m_width, m_height - i, i, 1)] = 0;	m_outBuffer[INDEX(m_width, m_height - i, i, 2)] = 1;
	}
}
//==========


////===Transformation Setters===
void Renderer::setCameraTransform(const mat4& tc) { this->tc = tc; }
void Renderer::setProjection(const mat4& tp) { this->tp = tp; }
void Renderer::setWorldTransform(const mat4& tw) { this->tw = tw; }
////==========


void Renderer::drawModel(MeshModel& model) {
	mat4 tm_tot = tp * tc * tw * model.tm;
	vector<Vertex> tr_vertices;
	vector<Pixel> px_vertices;
	for (vector<Vertex>::iterator i = model.vertices.begin(); i != model.vertices.end(); i++) {
		vec4 v = tm_tot * (*i);
		tr_vertices.push_back(v);
		px_vertices.push_back(viewPort(v));
	}

	mat4 ntm_t1 = tw * model.ntm;
	mat4 ntm_t2 = tp * tc;
	vector<Normal> tr_vertex_normals;
	for (vector<Normal>::iterator i = model.vertex_normals.begin(); i != model.vertex_normals.end(); i++) {
		vec4 n = ntm_t1 * (*i);
		n.w = 0;
		n = normalize(n);
		n.w = 1;
		n = ntm_t2 * n;
		tr_vertex_normals.push_back(n);
	}

	vector<Normal> tr_face_normals;
	for (vector<Normal>::iterator i = model.face_normals.begin(); i != model.face_normals.end(); i++) {
		vec4 n = ntm_t1 * (*i);
		n.w = 0;
		n = normalize(n);
		n.w = 1;
		n = ntm_t2 * n;
		tr_face_normals.push_back(n);
	}

	if (model.draw_pref.poly_mode == DrawPref::VERTICES_ONLY) {
		for (vector<Pixel>::iterator i = px_vertices.begin(); i != px_vertices.end(); i++) {
			drawPixel(*i, model.mesh_color);
		}
	}

	for (vector<Face>::iterator i = model.faces.begin(); i != model.faces.end(); i++) {
		if (model.draw_pref.poly_mode == DrawPref::EDGES_ONLY) {
			int* indexes = i->vertices;
			for (int j = 0; j < 3; j++) {
				Line l = Line(px_vertices[indexes[j]-1], px_vertices[indexes[(j+1)%3]-1]);
				drawLine(l, model.mesh_color);
			}
		}

		if (model.draw_pref.poly_mode == DrawPref::FILLED) {
			Triangle t = Triangle(px_vertices[i->vertices[0] - 1], px_vertices[i->vertices[1] - 1], px_vertices[i->vertices[2] - 1]);
			drawTriangleSolid(t, model.mesh_color);
		}

		if (model.draw_pref.f_draw_vertex_normals) {
			for (int j = 0; j < 3; j++) {
				Pixel start = px_vertices[i->vertices[j]-1];
				Vertex end_v = tr_vertices[i->vertices[j] - 1] + tr_vertex_normals[i->vertex_normals[j] - 1];
				end_v.w = 1;
				Pixel end = viewPort(end_v);
				drawLine(Line(start, end), model.vertex_normals_color);
			}
		}

		if (model.draw_pref.f_draw_faces_normals) {
			Vertex center = (tr_vertices[i->vertices[0]-1] + tr_vertices[i->vertices[1]-1] + tr_vertices[i->vertices[2]-1]) / 3;
			Pixel start = viewPort(center);
			Vertex end_v = center + tr_face_normals[i->normal];
			end_v.w = 1;
			Pixel end = viewPort(end_v);
			drawLine(Line(start, end), model.face_normals_color);
		}
	}

	if (model.draw_pref.f_draw_bb) {
		for (vector<Edge>::iterator i = model.bb_edges.begin(); i != model.bb_edges.end(); i++) {
			drawLine(Line(viewPort(tm_tot * i->start), viewPort(tm_tot * i->end)), model.bb_color);
		}
	}
}

void Renderer::drawOrigin(Color c){

	vec4 v = vec4(0,0,0,1);
	mat4 t_tot = tp * tc * tw;
	drawPixel(viewPort(t_tot * v), c);

}