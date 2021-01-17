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

	glGenBuffers(BUFFERS_COUNT, vbos);

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
	vector<Normal> vertex_textures;
	vector<Normal> face_normals;
	vector<Edge> bb_edges;
	vector<vec2> textures;


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
			textures.push_back(vec2fFromStream(issLine));
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
		Normal n = cross(e0, e1);
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

		vertex_normals_buff[3 * i] = vertex_normals[faces[i].vertices[0] - 1];
		vertex_normals_buff[(3 * i) + 1] = vertex_normals[faces[i].vertices[1] - 1];
		vertex_normals_buff[(3 * i) + 2] = vertex_normals[faces[i].vertices[2] - 1];

		face_normals_buff[3 * i] = face_normals[faces[i].normal];
		face_normals_buff[(3 * i) + 1] = face_normals[faces[i].normal];
		face_normals_buff[(3 * i) + 2] = face_normals[faces[i].normal];

		vertex_textures_buff[3 * i] = textures[faces[i].textures[0] - 1];
		vertex_textures_buff[(3 * i) + 1] = textures[faces[i].textures[1] - 1];
		vertex_textures_buff[(3 * i) + 2] = textures[faces[i].textures[2] - 1];
	}

	glBindBuffer(GL_ARRAY_BUFFER, vbos[BT_VERTICES]);
	glBufferData(GL_ARRAY_BUFFER, vertex_positions_tot_size + vertex_normals_tot_size + face_normals_tot_size + vertex_textures_tot_size, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, vertex_positions_tot_size, vertex_positions_buff);
	glBufferSubData(GL_ARRAY_BUFFER, vertex_positions_tot_size, vertex_normals_tot_size, vertex_normals_buff);
	glBufferSubData(GL_ARRAY_BUFFER, vertex_positions_tot_size + vertex_normals_tot_size, face_normals_tot_size, face_normals_buff);
	glBufferSubData(GL_ARRAY_BUFFER, vertex_positions_tot_size + vertex_normals_tot_size, face_normals_tot_size + vertex_textures_tot_size, vertex_textures_buff);

	//glBindBuffer(GL_ARRAY_BUFFER, vbos[BT_NORMALS]);
	//gl

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

void MeshModel::draw(mat4 tpc, GLuint program) {
	


	mat4 vt = tpc * tw * tm;
	mat4 nt = ntw * ntm;

	glBindVertexArray(vao);
	GLuint vt_loc = glGetUniformLocation(program, "v_transform");
	glUniformMatrix4fv(vt_loc, 1, GL_TRUE, vt);

	GLuint nt_loc = glGetUniformLocation(program, "n_transform");
	glUniformMatrix4fv(nt_loc, 1, GL_TRUE, nt);

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
	//glDrawArrays(GL_LINE, faces.size() * 3, faces.size() * 3);
	glFlush();

}