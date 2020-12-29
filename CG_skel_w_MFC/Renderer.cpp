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
	m_outBuffer = new float[3 * m_width * m_height];
	m_zbuffer = new float[m_width * m_height];
}
//==========


//===Inner Drawing Functions===

void Renderer::drawPixel(Pixel p, Color c) {
	if ((!isPixelLegal(p)) || p.z < m_zbuffer[Z_INDEX(m_width, p.x, p.y)]) {
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
			drawLineSteep(Line(p0, p1), c, pixels_drawn);
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
	int D = 2 * dy - dx;
	int y = p0.y;
	for (int x = p0.x; x <= p1.x; x++) {
		Pixel p = Pixel(x, y, z);
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

void Renderer::drawLineSteep(Line l, Color c, vector<Pixel>* pixels_drawn) {
	//flip x<->y
	Pixel p0 = { l.start.y, l.start.x, l.start.z };
	Pixel p1 = { l.end.y, l.end.x, l.end.z };
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
	const GLfloat vtc[] = {
		-1, -1,
		1, -1,
		-1, 1,
		-1, 1,
		1, -1,
		1, 1
	};
	const GLfloat tex[] = {
		0,0,
		1,0,
		0,1,
		0,1,
		1,0,
		1,1 };
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vtc) + sizeof(tex), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vtc), vtc);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vtc), sizeof(tex), tex);

	GLuint program = InitShader("vshader.glsl", "fshader.glsl");
	glUseProgram(program);
	GLint  vPosition = glGetAttribLocation(program, "vPosition");

	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 2, GL_FLOAT, GL_FALSE, 0,
		0);

	GLint  vTexCoord = glGetAttribLocation(program, "vTexCoord");
	glEnableVertexAttribArray(vTexCoord);
	glVertexAttribPointer(vTexCoord, 2, GL_FLOAT, GL_FALSE, 0,
		(GLvoid*)sizeof(vtc));
	glProgramUniform1i(program, glGetUniformLocation(program, "texture"), 0);
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


//===C'tor/D'tor===
Renderer::Renderer(int width, int height) :m_width(width), m_height(height),
f_anti_aliasing(false), fog({ {1,1,1}, 0.5 }), f_fog(false), f_axes(true)
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
	if (f_fog) {
		applyFog();
	}
	int a = glGetError();
	glActiveTexture(GL_TEXTURE0);
	a = glGetError();
	glBindTexture(GL_TEXTURE_2D, gScreenTex);
	a = glGetError();

	float* anti_aliased_buffer;
	if (f_anti_aliasing) {
		anti_aliased_buffer = createAntiAliasedBuffer();
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_width / 2, m_height / 2, GL_RGB, GL_FLOAT, anti_aliased_buffer);
	}
	else {
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_width, m_height, GL_RGB, GL_FLOAT, m_outBuffer);
	}

	glGenerateMipmap(GL_TEXTURE_2D);
	a = glGetError();

	glBindVertexArray(gScreenVtc);
	a = glGetError();
	glDrawArrays(GL_TRIANGLES, 0, 6);
	a = glGetError();
	glutSwapBuffers();
	a = glGetError();

	if (f_anti_aliasing) {
		delete[] anti_aliased_buffer;
	}
}

void Renderer::clearBuffer() {
	for (int i = 0; i < M_OUT_BUFFER_SIZE; i++) {
		m_outBuffer[i] = 0.2;
	}
	for (int i = 0; i < M_ZBUFFER_SIZE; i++) {
		m_zbuffer[i] = -1;
	}
}

void Renderer::colorBackground(Color color) {
	for (int i = 0; i < M_OUT_BUFFER_SIZE; i += 3) {
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
	if (f_anti_aliasing) {
		m_height *= 2;
		m_width *= 2;
	}
	createBuffers();
}

//==========

void Renderer::drawCamera(vec4 pos, Color c) {
	mat4 t_tot = camera->tp * camera->tc;
	Pixel center = viewPort(t_tot * pos);
	for (int j = -2; j <= 2; j++) {
		drawPixel(Pixel(center.x + j, center.y, center.z), c);
		drawPixel(Pixel(center.x, center.y + j, center.z), c);
	}
}

void Renderer::SetDemoBuffer()
{
	//vertical line
	for (int i = 0; i < m_width; i++)
	{
		m_outBuffer[INDEX(m_width, 256, i, 0)] = 1;	m_outBuffer[INDEX(m_width, 256, i, 1)] = 0;	m_outBuffer[INDEX(m_width, 256, i, 2)] = 0;
	}
	//horizontal line
	for (int i = 0; i < m_height; i++)
	{
		m_outBuffer[INDEX(m_width, i, 256, 0)] = 1;	m_outBuffer[INDEX(m_width, i, 256, 1)] = 0;	m_outBuffer[INDEX(m_width, i, 256, 2)] = 1;
	}

	//diagonal line
	for (int i = 0; i < m_height; i++)
	{
		m_outBuffer[INDEX(m_width, m_height - i, i, 0)] = 1;	m_outBuffer[INDEX(m_width, m_height - i, i, 1)] = 0;	m_outBuffer[INDEX(m_width, m_height - i, i, 2)] = 1;
	}
}
//==========


////===Transformation Setters===
//void Renderer::setCameraTransform(const mat4& tc) { this->tc = tc; }
//void Renderer::setProjection(const mat4& tp) { this->tp = tp; }
//void Renderer::setWorldTransform(const mat4& tw) { this->tw = tw; }
void Renderer::setActiveCameraPosition(vec4 pos) { this->active_camera_pos = pos; }
////==========


void Renderer::setAmbientColor(Color* color) { scene_ambient_light_color = color; }
////==========


void Renderer::drawModel(MeshModel& model) {


	mat4 tm_tot = camera->tp * camera->tc * model.tw* model.tm;
	vector<Vertex> tr_vertices;
	vector<Pixel> px_vertices;
	for (vector<Vertex>::iterator i = model.vertices.begin(); i != model.vertices.end(); i++) {
		vec4 v = tm_tot * (*i);
		v /= v.w;
		tr_vertices.push_back(v);
		px_vertices.push_back(viewPort(v));
	}

	mat4 ntm_t1 = model.ntw *model.ntm;
	mat4 ntm_t2 = camera->tpn * camera->tcn;
	vector<Normal> tr_vertex_normals;
	for (vector<Normal>::iterator i = model.vertex_normals.begin(); i != model.vertex_normals.end(); i++) {
		vec4 n = ntm_t1 * (*i);
		n /= n.w;
		n.w = 0;
		n = normalize(n)/100;
		n.w = 1;
		n = ntm_t2 * n;
		n /= n.w;
		tr_vertex_normals.push_back(n);
		/*vec4 n = ntm_t2 * ntm_t1 * (*i);
		n /= n.w;
		n.w = 0;
		n = normalize(n)/10;
		n.w = 1;
		tr_vertex_normals.push_back(n);*/
	}

	mat4 I = transpose(ntm_t2 * ntm_t1) * tm_tot;

	Color model_ambient_color = calculateAmbientColor(model);
	vector<Normal> tr_face_normals;
	for (vector<Normal>::iterator i = model.face_normals.begin(); i != model.face_normals.end(); i++) {
		vec4 n = ntm_t1 * (*i);
		n /= n.w;
		n.w = 0;
		n = normalize(n)/100;
		n.w = 1;
		n = ntm_t2 * n;
		n /= n.w;
		tr_face_normals.push_back(n);
		/*vec4 n = ntm_t2 * ntm_t1 * (*i);
		n /= n.w;
		n.w = 0;
		n = normalize(n)/10;
		n.w = 1;
		tr_face_normals.push_back(n);*/
	}

	if (model.draw_pref.poly_mode == DrawPref::VERTICES_ONLY) {
		for (vector<Pixel>::iterator i = px_vertices.begin(); i != px_vertices.end(); i++) {
			drawPixel(*i, model.mesh_color);
		}
	}

	for (vector<Face>::iterator i = model.faces.begin(); i != model.faces.end(); i++) {

		Vertex center = tm_tot * i->center;
		center = center / center.w;

		if (model.draw_pref.poly_mode == DrawPref::EDGES_ONLY) {
			int* indexes = i->vertices;
			for (int j = 0; j < 3; j++) {
				Line l = Line(px_vertices[indexes[j] - 1], px_vertices[indexes[(j + 1) % 3] - 1]);
				drawLine(l, model.mesh_color);
			}
		}

		if (model.draw_pref.poly_mode == DrawPref::FILLED) {
			if (model.draw_pref.shading == DrawPref::FLAT) {
				Triangle t = Triangle(px_vertices[i->vertices[0] - 1], px_vertices[i->vertices[1] - 1], px_vertices[i->vertices[2] - 1]);
				Normal normal = tr_face_normals[i->normal];
				Color face_diffuse_color = calculateDiffuseColor(model, center, normal);
				vec4 dir_to_camera = active_camera_pos - center;
				Color face_specular_color = calculateSpecularColor(model, center, normal, dir_to_camera);
				Color face_final_color = model_ambient_color + face_diffuse_color + face_specular_color;
				face_final_color.floorToOne();
				drawTriangleSolid(t, face_final_color);
			}
			if (model.draw_pref.shading == DrawPref::GOURAUD) {

			}

			if (model.draw_pref.shading == DrawPref::PHONG) {


			}


		}

		if (model.draw_pref.f_draw_vertex_normals) {
			for (int j = 0; j < 3; j++) {
				Pixel start = px_vertices[i->vertices[j] - 1];
				Vertex end_v = tr_vertices[i->vertices[j] - 1] + tr_vertex_normals[i->vertex_normals[j] - 1];
				end_v.w = 1;
				Pixel end = viewPort(end_v);
				drawLine(Line(start, end), model.vertex_normals_color);
			}
		}

		if (model.draw_pref.f_draw_faces_normals) {
			//Vertex center = (tr_vertices[i->vertices[0] - 1] + tr_vertices[i->vertices[1] - 1] + tr_vertices[i->vertices[2] - 1]) / 3;
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

void Renderer::drawOrigin(Color c) {
	vec4 v = vec4(0, 0, 0, 1);
	mat4 t_tot = camera->tp * camera->tc;
	drawPixel(viewPort(t_tot * v), c);
}

void Renderer::toggleAntiAliasing() {

	f_anti_aliasing = !f_anti_aliasing;
	if (f_anti_aliasing) {
		m_width *= 2;
		m_height *= 2;
	}
	else {
		m_width /= 2;
		m_height /= 2;
	}
	createBuffers();
}


float* Renderer::createAntiAliasedBuffer() {
	float* new_buff = new float[(m_height * m_width * 3) / 4];

	for (int y = 0; y < m_height / 2; y++) {
		for (int x = 0; x < m_width / 2; x++) {
			for (int k = 0; k < 3; k++) {
				new_buff[INDEX((m_width / 2), x, y, k)] =
					(m_outBuffer[INDEX(m_width, (x * 2), (y * 2), k)] +
						m_outBuffer[INDEX(m_width, ((x * 2) + 1), (y * 2), k)] +
						m_outBuffer[INDEX(m_width, (x * 2), ((y * 2) + 1), k)] +
						m_outBuffer[INDEX(m_width, ((x * 2) + 1), ((y * 2) + 1), k)]) / 4;
			}
		}
	}
	return new_buff;
}


////===Lighting Calculations===
Color Renderer::calculateAmbientColor(MeshModel& m) {
	return *scene_ambient_light_color * m.ambient_color;
}

Color Renderer::calculateDiffuseColor(MeshModel& m, Vertex center, Normal normal) {
	Color diffuse_color = { 0,0,0 };
	float factor;
	
	for (vector<ParallelSource>::iterator i = parallel_sources.begin(); i != parallel_sources.end(); i++) {
		vec4 dir = i->getDirection();

		vec3 v0 = normalize(vec3(normal.x, normal.y, normal.z));
		vec3 v1 = -normalize(vec3(dir.x, dir.y, dir.z));

		factor = max(0, v0 * v1);
		diffuse_color += (m.diffuse_color * i->getColor()) * factor;
	}


	for (vector<PointSource>::iterator i = point_sources.begin(); i != point_sources.end(); i++) {
		vec4 dir = center - i->getPosition();

		vec3 v0 = normalize(vec3(normal.x, normal.y, normal.z));
		vec3 v1 = -normalize(vec3(dir.x, dir.y, dir.z));

		factor = max(0, v0 * v1);
		diffuse_color += (m.diffuse_color * i->getColor()) * factor;
	}

	return diffuse_color;
}

Color Renderer::calculateSpecularColor(MeshModel& m, Vertex point, Normal normal, vec4 dir_to_camera) {
	Color specular_color = { 0,0,0 };
	float factor;

	for (vector<PointSource>::iterator i = point_sources.begin(); i != point_sources.end(); i++) {
		vec4 l = normalize(point - i->getPosition());
		l.w = 1;
		vec4 n = normalize(vec4(normal.x, normal.y, normal.z));
		vec4 r = 2 * (l * n) * n - l;

		factor = max(0, pow((r * dir_to_camera), m.shininess));
		specular_color += (m.diffuse_color * i->getColor()) * factor;
	}

	return specular_color;
}
////==========


void Renderer::toggleFog() {
	f_fog = !f_fog;
}

void Renderer::applyFog() {
	for (int x = 0; x < m_width; x++) {
		for (int y = 0; y < m_height; y++) {
			Color c = { m_outBuffer[INDEX(m_width, x, y, 0)], m_outBuffer[INDEX(m_width, x, y, 1)], m_outBuffer[INDEX(m_width, x, y, 2)] };
			float fog_percent = ((1 - m_zbuffer[Z_INDEX(m_width, x, y)]) * fog.max_density) / 2;
			c = (c * (1 - fog_percent)) + (fog.c * fog_percent);
			m_outBuffer[INDEX(m_width, x, y, 0)] = c.r;
			m_outBuffer[INDEX(m_width, x, y, 1)] = c.g;
			m_outBuffer[INDEX(m_width, x, y, 2)] = c.b;
		}
	}
}

void Renderer::drawAxes() {
	mat4 t_tot = camera->tp * camera->tc;
	vec4 axes[3];
	axes[0] = t_tot * vec4(1, 0, 0, 1);
	axes[1] = t_tot * vec4(0, 1, 0, 1);
	axes[2] = t_tot * vec4(0, 0, 1, 1);

	Color colors[3];
	colors[0] = { 1,0,0 };
	colors[1] = { 0,1,0 };
	colors[2] = { 0,0,1 };

	vec4 origin = vec4(-0.8, 0.8, 0, 1);


	for (int i = 0; i < 3; i++) {
		axes[i] /= axes[i].w;
		axes[i].w = 0;
		axes[i] = normalize(axes[i]) / 10;

		drawLine(Line(viewPort(origin), viewPort(origin + axes[i])), colors[i]);

	}

}

void Renderer::applyBlur(float* buffer) {
	float weight[9] = { 0.016216,
						0.054054,
						0.1216216,
						0.1945946,
						0.227027,
						0.1945946,
						0.1216216,
						0.054054,
						0.016216,
	};

	int buff_size = m_width * m_height * 3;
	float* tmp_buff = new float[buff_size];

	for (int i = 0; i < buff_size; i++) {
		tmp_buff[i] = buffer[i];
		buffer[i] = 0;
	}

	//horizontal blur
	for (int x = 0; x < m_width; x++) {
		for (int y = 0; y < m_height; y++) {
			for (int k = -4; k <= 4; k++) {
				if (x + k >= 0 && x + k < m_width) {
					for (int c = 0; c < 3; c++) {
						buffer[INDEX(m_width, (x + k), y, c)] += tmp_buff[INDEX(m_width, x, y, c)] * weight[4 + k];
					}
				}
			}
		}
	}

	for (int i = 0; i < buff_size; i++) {
		tmp_buff[i] = buffer[i];
		buffer[i] = 0;
	}

	//vertical blur
	for (int x = 0; x < m_width; x++) {
		for (int y = 0; y < m_height; y++) {
			for (int k = -4; k <= 4; k++) {
				if (y + k >= 0 && y + k < m_height) {
					for (int c = 0; c < 3; c++) {
						buffer[INDEX(m_width, x, (y + k), c)] += tmp_buff[INDEX(m_width, x, y, c)] * weight[4 + k];
					}
				}
			}
		}
	}

	delete tmp_buff;
}

void Renderer::setLightSources(vector<PointSource> points, vector<ParallelSource> parallels) {
	parallel_sources.clear();
	point_sources.clear();

	mat4 t_tot = camera->tp * camera->tc;

	for (vector<PointSource>::iterator i = points.begin(); i != points.end(); i++) {
		point_sources.push_back(t_tot * (*i));
	}

	for (vector<ParallelSource>::iterator i = parallels.begin(); i != parallels.end(); i++) {
		parallel_sources.push_back(t_tot * (*i));
	}
}

void Renderer::applyBloom(float bloom_threshold) {
	int buff_size = m_width * m_height * 3;
	float* bloom_buff = new float[buff_size];

	for (int i = 0; i < buff_size; i++) {
		bloom_buff[i] = 0;
	}

	for (int i = 0; i < buff_size; i += 3) {
		if (m_outBuffer[i] > bloom_threshold
			|| m_outBuffer[i + 1] > bloom_threshold
			|| m_outBuffer[i + 2] > bloom_threshold) {
			bloom_buff[i] = m_outBuffer[i];
			bloom_buff[i + 1] = m_outBuffer[i + 1];
			bloom_buff[i + 2] = m_outBuffer[i + 2];
		}
	}

	applyBlur(bloom_buff);

	for (int i = 0; i < buff_size; i++) {
		m_outBuffer[i] = max(0, m_outBuffer[i] + bloom_buff[i]);
	}

	delete bloom_buff;
}


void Renderer::setCamera(Camera* camera) { this->camera = camera; }