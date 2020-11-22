#include "StdAfx.h"
#include "Renderer.h"
#include "CG_skel_w_MFC.h"
#include "InitShader.h"
#include "GL\freeglut.h"
#include "MeshModel.h"

#define INDEX(width,x,y,c) (x+y*width)*3+c


//===Buffer Functions===
void Renderer::CreateBuffers()
{
	CreateOpenGLBuffer(); //Do not remove this line.
	m_outBuffer = new float[3*m_width*m_height];
}
//==========


//===Inner Drawing Functions===

void Renderer::rasterizePoint(Pixel p, Color c) {
	if (!isPixelLegal(p)) {
		return;
	}
	m_outBuffer[INDEX(m_width, p.x, p.y, 0)] = c.r;
	m_outBuffer[INDEX(m_width, p.x, p.y, 1)] = c.g;
	m_outBuffer[INDEX(m_width, p.x, p.y, 2)] = c.b;
}

void Renderer::rasterizeLine(Line l, Color c) {
	Pixel p0 = l.start;
	Pixel p1 = l.end;
	if (p0.x == p1.x) {
		if (p0.y < p1.y) {
			drawLineSteep(Line(p0,p1), c);
		}
		else {
			drawLineSteep(Line(p1, p0), c);
		}
	}
	if (p0.x > p1.x) {// make sure that v0 is left of v1
		p0 = l.end;
		p1 = l.start;
	}

	if (std::abs(p1.y - p0.y) < std::abs(p1.x - p0.x)) {//-1 <= m <= 1
		drawLineModerate(Line(p0, p1), c);
	}
	else { //|m|>1
		drawLineSteep(Line(p0, p1), c);
	}
}


void Renderer::drawLineModerate(Line l, Color c) {
	Pixel p0 = l.start;
	Pixel p1 = l.end;
	int dx = p1.x - p0.x;
	int dy = p1.y - p0.y;
	int yi = 1;
	if (dy < 0) {
		yi = -1;
		dy = -dy;
	}
	int D = 2*dy - dx;
	int y = p0.y;
	for (int x = p0.x; x <= p1.x; x++) {
		this->rasterizePoint(Pixel(x,y), c);
		if (D > 0) {
			y += yi;
			D += 2*(dy-dx);
		}
		else {
			D += 2*dy;
		}
	}
}

void Renderer::drawLineSteep(Line l, Color c) {
	//flip x<->y
	Pixel p0 = { l.start.y, l.start.x };
	Pixel p1 = { l.end.y, l.end.x };
	//make sure p0 is left of p1
	if (p0.x > p1.x) {
		Pixel tmp = p0;
		p0 = p1;
		p1 = tmp;
	}
	int dx = p1.x - p0.x;
	int dy = p1.y - p0.y;
	int yi = 1;
	if (dy < 0) {
		yi = -1;
		dy = -dy;
	}
	int D = 2 * dy - dx;
	int y = p0.y;
	for (int x = p0.x; x <= p1.x; x++) {
		this->rasterizePoint(Pixel(y,x), c);
		if (D > 0) {
			y += yi;
			D += 2 * (dy - dx);
		}
		else {
			D += 2 * dy;
		}
	}
}
//==========


//===Inner Calculations===

bool Renderer::isPixelLegal(Pixel p) {
	return p.x >= 0 && p.x < m_width && p.y >= 0 && p.y < m_height;
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
		(int)std::round((m_height * (v.y + 1)) / 2)
	};
	return p;
}

vector<Pixel> Renderer::transformVertices(vector<Vertex>& vertices, mat4 tm){ //TODO: improve clipping
	vector<Pixel> pixels;
	mat4 t_tot = tp * tc * tw * tm;
	for (vector<vec4>::iterator i = vertices.begin(); i != vertices.end(); i++) {
		vec4 v = t_tot * (*i);
		Pixel p = viewPort(v);
		if (isPixelLegal(p)) {
			pixels.push_back(p);
		}
	}
	return pixels;
}


vector<Line> Renderer::transformEdges(vector<vec4>& edges, mat4 tm) { //TODO:delete
	vector<Line> lines;
	mat4 t_tot = tp * tc * tw * tm;
	for (vector<vec4>::iterator i = edges.begin(); i != edges.end(); i += 2) {
		vec4 v0 = t_tot * (*i);
		vec4 v1 = t_tot * (*(i + 1));
		Line l = Line(viewPort(v0), viewPort(v1));
		if (isLineLegal(l)) {
			lines.push_back(l);
		}
	}
	return lines;
}

vector<Line> Renderer::transformEdges(vector<Edge>& edges, mat4 tm) { //TODO: improve cliping
	vector<Line> lines;
	mat4 t_tot = tp * tc * tw * tm;
	for (vector<Edge>::iterator i = edges.begin(); i != edges.end(); i++) {
		vec4 v0 = t_tot * (i->start);
		vec4 v1 = t_tot * (i->end);
		Line l = Line(viewPort(v0),viewPort(v1));
		if (isLineLegal(l)) {
			lines.push_back(l);
		}
	}
	return lines;
}

vector<Line> Renderer::transformFaces(vector<vec4>& faces, mat4 tm) {
	vector<Line> lines;
	mat4 t_tot = tp * tc * tw * tm;
	for (vector<vec4>::iterator i = faces.begin(); i != faces.end(); i+=3) {
		for (int j = 0; j < 3; j++) {
			vec4 v0 = t_tot * (*(i + j));
			vec4 v1 = t_tot * (*(i + ((j + 1) % 3)));
			Line l = Line(viewPort(v0),viewPort(v1));
			if (isLineLegal(l)) {
				lines.push_back(l);
			}
		}
	}
	return lines;
}

vector<Line> Renderer::transformNormals(vector<Normal>& normals, mat4 tm, mat4 ntm) {
	vector<Line> lines;
	mat4 tm_tot = tp * tc * tw * tm;
	mat4 ntm_t1 = tw * ntm;
	mat4 ntm_t2 = tp * tc;
	for (int i = 0; i < normals.size(); i++) {
		vec4 start = tm_tot * (normals[i].vertex);
		start = start / start.w;
		vec4 direction = ntm_t1 * (normals[i].direction);
		direction.w = 0;
		direction = normalize(direction);
		direction.w = 1;
		direction = ntm_t2 * direction;
		direction = direction / direction.w;
		vec4 end = start + direction;
		end.w = 1;
		Line l = Line(viewPort(start), viewPort(end));
		if (isLineLegal(l)) {
			lines.push_back(l);
		}
	}
	return lines;
}

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
	CreateBuffers();
}

Renderer::~Renderer(void)
{
	delete[] m_outBuffer;
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

void Renderer::clearColorBuffer() {
	for (int i = 0; i < M_OUT_BUFFER_SIZE; i++) {
		m_outBuffer[i] = 0.2;
	}
}

void Renderer::colorBackground(Color color) {
	for (int i = 0; i < M_OUT_BUFFER_SIZE; i+=3) {
		m_outBuffer[i] = color.r;
		m_outBuffer[i + 1] = color.g;
		m_outBuffer[i + 2] = color.b;
	}
}


void Renderer::setSize(int width, int height) {
	delete[] m_outBuffer;
	m_width = width;
	m_height = height;
	CreateBuffers();
}
//==========


//===Drawing Interface===
void Renderer::drawPoints(vector<Vertex>& points, mat4 tm, Color c){
	vector<Pixel> pixels = transformVertices(points, tm);
	for (vector<Pixel>::iterator i = pixels.begin(); i != pixels.end(); i++) {
		rasterizePoint(*i, c);
	}
}

void Renderer::drawLines(vector<vec4>& points, mat4 tm, Color c) {
	vector<Line> lines = transformEdges(points, tm);
	for (vector<Line>::iterator i = lines.begin(); i != lines.end(); i++) {
		rasterizeLine(*i, c);
	}
}

void Renderer::drawLines(vector<Edge>& edges, mat4 tm, Color c) {//TODO: implement
	vector<Line> lines = transformEdges(edges, tm);
	for (vector<Line>::iterator i = lines.begin(); i != lines.end(); i++) {
		rasterizeLine(*i, c);
	}
}

void Renderer::drawTriangles(vector<vec4>& vertex_positions, mat4 tm, Color c) {
	vector<Line> lines = transformFaces(vertex_positions, tm);
	for (vector<Line>::iterator i = lines.begin(); i != lines.end(); i++) {
		rasterizeLine(*i, c);
	}
}

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

void Renderer::drawVertexNormals(vector<Normal>& normals, mat4 tm, mat4 ntm, Color c) {
	vector<Line> lines = transformNormals(normals, tm, ntm);
	for (vector<Line>::iterator i = lines.begin(); i != lines.end(); i++) {
		rasterizeLine(*i, c);
	}
}

void Renderer::drawFacesNormals(vector<Normal>& normals, mat4 tm, mat4 ntm, Color c) {
	vector<Line> lines = transformNormals(normals, tm, ntm);
	for (vector<Line>::iterator i = lines.begin(); i != lines.end(); i++) {
		rasterizeLine(*i, c);
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


//===Transformation Setters===
void Renderer::setCameraTransform(const mat4& tc) { this->tc = tc; }
void Renderer::setProjection(const mat4& tp) { this->tp = tp; }
void Renderer::setWorldTransform(const mat4& tw) { this->tw = tw; }
//==========
