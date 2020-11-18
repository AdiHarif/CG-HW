#include "StdAfx.h"
#include "Renderer.h"
#include "CG_skel_w_MFC.h"
#include "InitShader.h"
#include "GL\freeglut.h"

#define INDEX(width,x,y,c) (x+y*width)*3+c


//===Buffer Functions===
void Renderer::CreateBuffers(int width, int height)
{
	m_width=width;
	m_height=height;	
	CreateOpenGLBuffer(); //Do not remove this line.
	m_outBuffer = new float[3*m_width*m_height];
}
//==========


//===Transformations Calcs
void Renderer::calcViewport(){
	mat4 s = scaleMat(m_width / 2.0, m_height / 2.0, 1);
	mat4 t = translateMat(m_width / 2.0, m_height / 2.0, 0);
	viewport = t * s;
}
//==========


//===Inner Drawing Functions===
void Renderer::drawLineModerate(vec4 v0, vec4 v1, Color c) {
	int dx = v1.x - v0.x;
	int dy = v1.y - v0.y;
	int yi = 1;
	if (dy < 0) {
		yi = -1;
		dy = -dy;
	}
	int D = 2*dy - dx;
	int y = v0.y;
	for (int x = v0.x; x <= v1.x; x++) {
		this->rasterizePoint(vec4(x, y, 0.0), c);
		if (D > 0) {
			y += yi;
			D += 2*(dy-dx);
		}
		else {
			D += 2*dy;
		}
	}
}

void Renderer::drawLineSteep(vec4 v0, vec4 v1, Color c) {
	//flip x<->y
	v0 = vec4(v0.y, v0.x, 0.0);
	v1 = vec4(v1.y, v1.x, 0.0);
	//make sure v0 is left of v1
	if (v0.x > v1.x) {
		vec4 tmp = v0;
		v0 = v1;
		v1 = tmp;
	}
	int dx = v1.x - v0.x;
	int dy = v1.y - v0.y;
	int yi = 1;
	if (dy < 0) {
		yi = -1;
		dy = -dy;
	}
	int D = 2 * dy - dx;
	int y = v0.y;
	for (int x = v0.x; x <= v1.x; x++) {
		this->rasterizePoint(vec4(y, x, 0.0), c);
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
	CreateBuffers(width,height);
	calcViewport();
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

//==========


//===Drawing Interface===
void Renderer::rasterizePoint(vec4 v, Color c){
	int x = std::round(v.x);
	int y = std::round(v.y);   
	if (x < 0 || x >= m_width || y < 0 || y >= m_height) {
		return;
	}
	m_outBuffer[INDEX(m_width, x, y, 0)] = c.r;
	m_outBuffer[INDEX(m_width, x, y, 1)] = c.g;
	m_outBuffer[INDEX(m_width, x, y, 2)] = c.b;
}

void Renderer::rasterizeLine(vec4 v0, vec4 v1, Color c) {
	if (v0.x == v1.x) {
		if (v0.y < v1.y) {
			drawLineSteep(v0, v1, c);
		}
		else {
			drawLineSteep(v1, v0, c);
		}
	}
	if (v0.x > v1.x) {// make sure that v0 is left of v1
		vec4 tmp = v0;
		v0 = v1;
		v1 = tmp;
	}

	if (std::abs(v1.y - v0.y) < std::abs(v1.x - v0.x)) {//-1 <= m <= 1
		drawLineModerate(v0, v1, c);
	}
	else { //|m|>1
		drawLineSteep(v0, v1, c);
	}
}

void Renderer::drawPoints(vector<vec4>& points, Color c){
	for (vector<vec4>::iterator i = points.begin(); i!= points.end(); i++){
		rasterizePoint(*i, c);
	}
}

void Renderer::drawTriangles(vector<vec4>& vertex_positions, Color c) {
	for (vector<vec4>::iterator i = vertex_positions.begin(); i != vertex_positions.end(); i+=3) {
		rasterizeLine(*i, *(i + 1), c);
		rasterizeLine(*(i + 1), *(i + 2), c);
		rasterizeLine(*(i + 2), *i, c);
	}
}

void Renderer::drawVertexNormals(vector<vec4>& vertices, vector<vec4>& vertex_normals, Color c) {
	/*for (vector<vec4>::iterator i = vertex_normals.begin(); i != vertex_normals.end(); i++) {
		rasterizeLine()
	}*/
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


//===Getters===
mat4 Renderer::getViewport() { return viewport; }
//==========
