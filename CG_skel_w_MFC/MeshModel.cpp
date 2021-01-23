#include "StdAfx.h"
#include "MeshModel.h"
#include "vec.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Face.h"

using namespace std;

vec3 vec3fFromStream(std::istream & aStream)
{
	float x, y, z;
	aStream >> x >> std::ws >> y >> std::ws >> z;
	return vec3(x, y, z);
}

vec2 vec2fFromStream(std::istream & aStream)
{
	float x, y;
	aStream >> x >> std::ws >> y;
	return vec2(x, y);
}

vec4 vec4fFromStream(std::istream& aStream) {
	return vec4(vec3fFromStream(aStream));
}

MeshModel::MeshModel(string file_name)
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	if (file_name != "") {
		loadFile(file_name);
	}
}

MeshModel::~MeshModel(void)
{
	//TODO: delete openGL buffers related to this model
}

void MeshModel::loadFile(string fileName)
{
	vec4 min, max;
	vector<Face> faces;
	vector<Vertex> vertices;
	vector<Normal> vertex_normals;
	vector<Normal> face_normals;
	vector<Edge> bb_edges;
	vector<vec2> vetrex_textures;


	ifstream ifile(fileName.c_str());
	// while not end of file
	while (!ifile.eof())
	{
		// get line
		string curLine;
		getline(ifile, curLine);

		// read type of the line
		istringstream issLine(curLine);
		string lineType;

		issLine >> std::ws >> lineType;

		// based on the type parse data
		if (lineType == "v"){
			vec4 cur_v = vec4fFromStream(issLine);
			min.x = (std::min)(min.x, cur_v.x);
			min.y = (std::min)(min.y, cur_v.y);
			min.z = (std::min)(min.z, cur_v.z);
			max.x = (std::max)(max.x, cur_v.x);
			max.y = (std::max)(max.y, cur_v.y);
			max.z = (std::max)(max.z, cur_v.z);
			vertices.push_back(cur_v);
		}
		else if (lineType == "f"){ 
			faces.push_back(issLine);
		} 
		else if (lineType == "vn") {
			vertex_normals.push_back(vec4fFromStream(issLine));
		}
		else if (lineType == "vt") {
			vec2 tmp = vec2fFromStream(issLine);
			tmp.y *= -1;
			vetrex_textures.push_back(tmp);
		}
		else if (lineType == "#" || lineType == "")
		{
			// comment / empty line
		}
		else
		{
			cout<< "Found unknown line Type \"" << lineType << "\"\n";
		}
	}
	
	initBoundingBox(min, max);
	
	//centering the model to (0,0,0):
	vec4 middle_offset = vec4((min.x + max.x) / 2, (min.y + max.y) / 2, (min.z + max.z) / 2);
	mat4 t = translateMat(-middle_offset);
	for (vector<vec4>::iterator i = vertices.begin(); i != vertices.end(); i++) {
		(*i) = t * (*i);
	}
	for (vector<Edge>::iterator i = bb_edges.begin(); i != bb_edges.end(); i++) {
		(*i) = t * (*i);
	}

	for (int i = 0; i < faces.size(); i++) {

		Face& f = faces[i];

		Vertex v0, v1, v2;
		v0 = vertices[f.vertices[0] - 1];
		v1 = vertices[f.vertices[1] - 1];
		v2 = vertices[f.vertices[2] - 1];

		f.calcCenter(v0, v1, v2);

		vec4 e0 = v1 - v0;
		vec4 e1 = v2 - v1;
		Normal n = normalize(cross((e0), (e1)));
		face_normals.push_back(n);
		faces[i].normal = i;

		faces[i].ambient_color = ambient_color;
		faces[i].diffuse_color = diffuse_color;
		faces[i].specular_color = specular_color;
		faces[i].emit_color = emit_color;
	}

	int vertex_positions_tot_size = faces.size() * 3 * sizeof(vec4);
	int vertex_normals_tot_size = vertex_positions_tot_size;
	int face_normals_tot_size = vertex_positions_tot_size;
	int vertex_textures_tot_size = faces.size() * 3 * sizeof(vec2);

	vec4* vertex_positions_buff = new vec4[faces.size() * 3];
	vec4* vertex_normals_buff = new vec4[faces.size() * 3];
	vec4* face_normals_buff = new vec4[faces.size() * 3];
	vec2* vertex_textures_buff = new vec2[faces.size() * 3];

	for (int i = 0; i < faces.size(); i++) {
		vertex_positions_buff[3 * i] = vertices[faces[i].vertices[0] - 1];
		vertex_positions_buff[(3 * i) + 1] = vertices[faces[i].vertices[1] - 1];
		vertex_positions_buff[(3 * i) + 2] = vertices[faces[i].vertices[2] - 1];

		vertex_normals_buff[3 * i] = vertex_normals[faces[i].vertex_normals[0] - 1];
		vertex_normals_buff[(3 * i) + 1] = vertex_normals[faces[i].vertex_normals[1] - 1];
		vertex_normals_buff[(3 * i) + 2] = vertex_normals[faces[i].vertex_normals[2] - 1];

		face_normals_buff[3 * i] = face_normals[faces[i].normal];
		face_normals_buff[(3 * i) + 1] = face_normals[faces[i].normal];
		face_normals_buff[(3 * i) + 2] = face_normals[faces[i].normal];

		if (vetrex_textures.size() > 0) {
			vertex_textures_buff[3 * i] = vetrex_textures[faces[i].textures[0] - 1];
			vertex_textures_buff[(3 * i) + 1] = vetrex_textures[faces[i].textures[1] - 1];
			vertex_textures_buff[(3 * i) + 2] = vetrex_textures[faces[i].textures[2] - 1];
		}
	}

	genVec4ArrayBuffer(BT_VERTICES, vertex_positions_tot_size, vertex_positions_buff);
	genVec4ArrayBuffer(BT_VERTEX_NORMALS, vertex_normals_tot_size, vertex_normals_buff);
	genVec4ArrayBuffer(BT_FACE_NORMALS, face_normals_tot_size, face_normals_buff);
	genVec2ArrayBuffer(BT_TEXTURES, vertex_textures_tot_size, vertex_textures_buff);


	//if (has_normals)
	//{
	//	glGenBuffers(1, &normals_buffer);
	//	glBindBuffer(GL_ARRAY_BUFFER, normals_buffer);
	//	glEnableVertexAttribArray(1);
	//	glBufferData(GL_ARRAY_BUFFER, sizeof(vec4) * vertex_positions_size,
	//		vertex_normals, GL_STATIC_DRAW);
	//	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
	//}

	//glGenBuffers(1, &face_normals_buffer);
	//glBindBuffer(GL_ARRAY_BUFFER, face_normals_buffer);
	//glEnableVertexAttribArray(2);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vec4) * vertex_positions_size,
	//	face_normals, GL_STATIC_DRAW);
	//glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 0, 0);



	delete[] vertex_positions_buff;
	delete[] vertex_normals_buff;
	delete[] face_normals_buff;

	faces_count = faces.size();
}

void MeshModel::translate(vec4 v, bool f_world_frame) {
	mat4 t = translateMat(v);
	mat4 nt = transpose(translateMat(-v));
	if (!f_world_frame) {
		tm = t * tm;
		//ntm = nt * ntm;
	}
	else {
		tw = t * tw;
		//ntw = nt * ntw;
	}
	//this->position = t * this->position;
	//this->tm = t * this->tm;
}

void MeshModel::scale(vec3 v, bool f_world_frame) {
	mat4 t = scaleMat(v);
	mat4 nt = scaleMat(vec3(1/v.x, 1/v.y, 1/v.z)); //bo beed for a transpose here
	if (!f_world_frame) {
		tm = t * tm;
		ntm = nt * ntm;
	}
	else {
		tw = t * tw;
		ntw = nt * ntw;
	}
	//mat4 s = scaleMat(v);
	//mat4 t1 = translateMat(this->position);
	//mat4 t2 = translateMat(-this->position);
	//this->tm = t1 * s * t2 * this->tm;

	// normals transformations: //TODO: fix
	/*mat4 sn1 = scaleMat(1 / v.x, 1 / v.y, 1 / v.z);
	ntm = sn1 * ntm;*/
}

void MeshModel::rotateX(GLfloat theta, bool f_world_frame) {
	mat4 t = rotateXMat(theta);
	mat4 nt = transpose(rotateXMat(-theta));
	if (!f_world_frame) {
		tm = t * tm;
		ntm = nt * ntm;
	}
	else {
		tw = t * tw;
		ntw = nt * ntw;
	}
	//mat4 r = rotateXMat(theta);
	//mat4 t1 = translateMat(this->position);
	//mat4 t2 = translateMat(-this->position);
	//this->tm = t1 * r * t2 * this->tm;

	//// normals transformations:
	//ntm =  r * ntm;
}

void MeshModel::rotateY(GLfloat theta, bool f_world_frame) {
	mat4 t = rotateYMat(theta);
	mat4 nt = transpose(rotateYMat(-theta));
	if (!f_world_frame) {
		tm = t * tm;
		ntm = nt * ntm;
	}
	else {
		tw = t * tw;
		ntw = nt * ntw;
	}
	//mat4 r = rotateYMat(theta);
	//mat4 t1 = translateMat(this->position);
	//mat4 t2 = translateMat(-this->position);
	//this->tm = t1 * r * t2 * this->tm;

	//// normals transformations:
	//ntm =  r * ntm;
}

void MeshModel::rotateZ(GLfloat theta, bool f_world_frame) {
	mat4 t = rotateZMat(theta);
	mat4 nt = transpose(rotateZMat(-theta));
	if (!f_world_frame) {
		tm = t * tm;
		ntm = nt * ntm;
	}
	else {
		tw = t * tw;
		ntw = nt * ntw;
	}
	//mat4 r = rotateZMat(theta);
	//mat4 t1 = translateMat(this->position);
	//mat4 t2 = translateMat(-this->position);
	//this->tm = t1 * r * t2 * this->tm;

	//// normals transformations:
	//ntm =  r * ntm;
}

//mat4 MeshModel::getWorldTransform() {
//	return this->tm;
//}

vec4 MeshModel::getPosition(){ 
	return tw * tm * vec4(0, 0, 0, 1);
}

//vector<Face>* MeshModel::getFaces() {
//	return &faces;
//}

void MeshModel::setTexture(const char* file_name) {
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &vto);
	glBindTexture(GL_TEXTURE_2D, vto);
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load and generate the texture
	int width, height, nrChannels;
	unsigned char* data = stbi_load(file_name, &width, &height, &nrChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
}

void MeshModel::initBoundingBox(vec4 min, vec4 max) {
	//vector<Vertex> bb_vertices;
	////bottom face:
	//bb_vertices.push_back(vec4(min.x, min.y, min.z));
	//bb_vertices.push_back(vec4(max.x, min.y, min.z));
	//bb_vertices.push_back(vec4(max.x, max.y, min.z));
	//bb_vertices.push_back(vec4(min.x, max.y, min.z));
	////upper face:
	//bb_vertices.push_back(vec4(min.x, min.y, max.z));
	//bb_vertices.push_back(vec4(max.x, min.y, max.z));
	//bb_vertices.push_back(vec4(max.x, max.y, max.z));
	//bb_vertices.push_back(vec4(min.x, max.y, max.z));

	//for (int i = 0; i < 4; i++) {
	//	bb_edges.push_back(Edge(bb_vertices[i], bb_vertices[(i + 1)%4]));
	//	bb_edges.push_back(Edge(bb_vertices[i+4], bb_vertices[((i+1)%4)+4]));
	//	bb_edges.push_back(Edge(bb_vertices[i], bb_vertices[i + 4]));
	//}
}

void MeshModel::setPolyMode(DrawPref::PolyMode poly_mode) {
	draw_pref.poly_mode = poly_mode;
}

void MeshModel::togglePolyMode() {
	draw_pref.poly_mode = DrawPref::PolyMode((draw_pref.poly_mode + 1) % 3);
}

void MeshModel::toggleBB(){
	draw_pref.f_draw_bb = !draw_pref.f_draw_bb;
}

void MeshModel::toggleVertexNormals(){
	draw_pref.f_draw_vertex_normals = !draw_pref.f_draw_vertex_normals;
}

void MeshModel::toggleFaceNormals(){
	draw_pref.f_draw_faces_normals = !draw_pref.f_draw_faces_normals;
}

void MeshModel::activate() {
	mesh_color = GREEN;
	vertex_normals_color = PURPLE;
	face_normals_color = RED;
	bb_color = WHITE;
}

void MeshModel::deactivate() {
	mesh_color = vertex_normals_color = face_normals_color = bb_color = INACTIVE_GRAY;
}

void MeshModel::draw() {
	switch (draw_pref.poly_mode) {
	case DrawPref::VERTICES_ONLY: {
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		break;
	}
	case DrawPref::EDGES_ONLY: {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		break;
	}
	case DrawPref::FILLED: {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	}
	}
	glDrawArrays(GL_TRIANGLES, 0, faces_count * 3);
	glFlush();
}

void MeshModel::genVec4ArrayBuffer(BufferType bt, int tot_size, vec4* buffer) {
	glGenBuffers(1, &vbos[bt]);
	glBindBuffer(GL_ARRAY_BUFFER, vbos[bt]);
	glBufferData(GL_ARRAY_BUFFER, tot_size,	buffer, GL_STATIC_DRAW);
}

void MeshModel::genVec2ArrayBuffer(BufferType bt, int tot_size, vec2* buffer) {
	glGenBuffers(1, &vbos[bt]);
	glBindBuffer(GL_ARRAY_BUFFER, vbos[bt]);
	glBufferData(GL_ARRAY_BUFFER, tot_size, buffer, GL_STATIC_DRAW);
}

void MeshModel::updateHSVColor() {
	hsv_color.r += (1.0f / 360);
	if (hsv_color.r > 1)	hsv_color.r = 0;
}

void MeshModel::updateWaveThreshold() {
	wave_threshold += WAVE_THRESHOLD_STEP;
	if (wave_threshold > 1) {
		wave_color_1 = wave_color_0;
		wave_color_0 = Color::getRandomColor();
		wave_threshold = -1;
	}
}

void MeshModel::updateVertexAnimationT() {
	if (current_vertex_animation_dir == VA_IN) {
		if(vertex_animation_x!=0)	vertex_animation_t = easeInCubic(vertex_animation_x);
		vertex_animation_x -= VERTEX_ANIMATION_X_STEP;
		if (vertex_animation_x < VERTEX_ANIMATION_X_MIN) {
			vertex_animation_x = VERTEX_ANIMATION_X_MIN;
			current_vertex_animation_dir = VA_OUT;
		}
	}
	else {
		vertex_animation_t = easeOutCubic(vertex_animation_x);
		vertex_animation_x += 2*VERTEX_ANIMATION_X_STEP;
		if (vertex_animation_x > VERTEX_ANIMATION_X_MAX) {
			vertex_animation_x = VERTEX_ANIMATION_X_MAX;
			current_vertex_animation_dir = VA_IN;
		}
	}
}

void MeshModel::resetVertexAnimationT() {
	vertex_animation_x = VERTEX_ANIMATION_X_MAX;
	vertex_animation_t = 0;
}