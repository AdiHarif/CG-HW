#include "StdAfx.h"
#include "MeshModel.h"
#include "vec.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>


using namespace std;

struct FaceIdcs
{
	int v[4];
	int vn[4];
	int vt[4];

	FaceIdcs()
	{
		for (int i=0; i<4; i++)
			v[i] = vn[i] = vt[i] = 0;
	}

	FaceIdcs(std::istream & aStream)
	{
		for (int i=0; i<4; i++)
			v[i] = vn[i] = vt[i] = 0;

		char c;
		for(int i = 0; i < 3; i++)
		{
			aStream >> std::ws >> v[i] >> std::ws;
			if (aStream.peek() != '/')
				continue;
			aStream >> c >> std::ws;
			if (aStream.peek() == '/')
			{
				aStream >> c >> std::ws >> vn[i];
				continue;
			}
			else
				aStream >> vt[i];
			if (aStream.peek() != '/')
				continue;
			aStream >> c >> vn[i];
		}
	}
};


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

MeshModel::MeshModel(string fileName)
{
	loadFile(fileName);
	mesh_color = { 0.0, 1.0, 0.0 };
	vertex_normals_color = { 1.0, 0.0, 1.0 };
	bb_color = { 1.0, 1.0, 1.0 };
	//scale(vec3(50, 50, 50));
}

MeshModel::~MeshModel(void)
{
	//delete[] vertex_positions;
}

void MeshModel::loadFile(string fileName)
{
	vec4 min, max;
	vector<FaceIdcs> faces;
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
		if (lineType == "v"){ /*BUG fixed*/
			vec4 cur_v = vec4fFromStream(issLine);
			min.x = (std::min)(min.x, cur_v.x);
			min.y = (std::min)(min.y, cur_v.y);
			min.z = (std::min)(min.z, cur_v.z);
			max.x = (std::max)(max.x, cur_v.x);
			max.y = (std::max)(max.y, cur_v.y);
			max.z = (std::max)(max.z, cur_v.z);
			this->vertices.push_back(cur_v);
			
		}
		else if (lineType == "f"){ /*BUG fixed*/
			faces.push_back(issLine);
		} 
		else if (lineType == "vn") {
			this->vertex_normals.push_back(vec4fFromStream(issLine));
		}
		else if (lineType == "#" || lineType == "")
		{
			// comment / empty line
		}
		else
		{
			cout<< "Found unknown line Type \"" << lineType << "\"";
		}
	}
	//Vertex_positions is an array of vec3. Every three elements define a triangle in 3D.
	//If the face part of the obj is
	//f 1 2 3
	//f 1 3 4
	//Then vertex_positions should contain:
	//vertex_positions={v1,v2,v3,v1,v3,v4}
	


	//setting model position:
	this->position = vec4(0.0);
	//centering the model to (0,0,0):
	vec4 middle_offset = vec4((min.x + max.x) / 2, (min.y + max.y) / 2, (min.z + max.z) / 2);
	mat4 t = translateMat(-middle_offset);
	for (vector<vec4>::iterator i = vertices.begin(); i != vertices.end(); i++) {
		(*i) = t * (*i);
	}

	// iterate through all stored faces and create triangles
	int k=0;
	for (vector<FaceIdcs>::iterator it = faces.begin(); it != faces.end(); ++it)
	{
		for (int i = 0; i < 3; i++)
		{
			//this->vertex_positions[k++] = vec3(this->vertices[it->v[i]-1]); /*BUG fixed*/
			this->vertex_positions.push_back(vec4(this->vertices[it->v[i] - 1]));
		}
	}

	this->initBoundingBox(min, max);

}

void MeshModel::draw(mat4 tcw, Renderer* renderer) {
	if(draw_pref.f_draw_vertices)	drawVertices(tcw, renderer);
	if(draw_pref.f_draw_edges)	drawEdges(tcw, renderer);
	if(draw_pref.f_draw_bb)	drawBoundingBox(tcw, renderer);
	if (draw_pref.f_draw_vertex_normals) drawVertexNormals(tcw, renderer);
}

void MeshModel::drawVertices(mat4 tcw, Renderer* renderer)
{
	mat4 tcwm = tcw * _world_transform;
	vector<vec4> vecs_to_draw; 
	for (vector<vec4>::iterator i = vertices.begin(); i != vertices.end(); i++){
		vecs_to_draw.push_back(tcwm*(*i));
	}
	renderer->drawPoints(vecs_to_draw, mesh_color);
}

void MeshModel::drawEdges(mat4 tcw, Renderer* renderer) {
	mat4 tcwm = tcw * _world_transform;
	vector<vec4> vecs_to_draw;
	for (vector<vec4>::iterator i = vertex_positions.begin(); i != vertex_positions.end(); i++) {
		vecs_to_draw.push_back(tcwm * (*i));
	}
	renderer->drawTriangles(vecs_to_draw, mesh_color);
}

void MeshModel::drawVertexNormals(mat4 tcw, Renderer* renderer) {
	mat4 tcwm = tcw * _world_transform;
	vector<vec4> vecs_to_draw;
	for (vector<vec4>::iterator i = vertex_normals.begin(); i != vertex_normals.end(); i++) {
		vecs_to_draw.push_back(tcwm * (*i));
	}
	//renderer->(vecs_to_draw, vertex_normals_color);
}

void MeshModel::drawBoundingBox(mat4 tcw, Renderer* renderer) {
	mat4 tcwm = tcw * _world_transform;
	vector<vec4> vecs_to_draw;
	for (vector<vec4>::iterator i = bounding_box_vertices.begin(); i != bounding_box_vertices.end(); i++) {
		vecs_to_draw.push_back(tcwm * (*i));
	}
	//draw only vertices (can delete later)
	renderer->drawPoints(vecs_to_draw, bb_color);
	//draw lines
	for (int i = 0; i < 4; i++) {//0->1, 1->2, 2->3, 3->0
		renderer->rasterizeLine(vecs_to_draw[i], vecs_to_draw[(i+1)%4], bb_color);
	}
	for (int i = 0; i < 4; i++) {//4->5, 5->6, 6->7, 7->4
		renderer->rasterizeLine(vecs_to_draw[i+4], vecs_to_draw[((i+1)%4)+4], bb_color);
	}
	for (int i = 0; i < 4; i++) {//0->4, 1->5, 2->6, 3->7
		renderer->rasterizeLine(vecs_to_draw[i], vecs_to_draw[i+4], bb_color);
	}
}

void MeshModel::translate(vec4 v) {
	mat4 t = translateMat(v);
	this->position = t * this->position;
	this->_world_transform = t * this->_world_transform;
}

void MeshModel::scale(vec3 v) {
	mat4 s = scaleMat(v);
	mat4 t1 = translateMat(this->position);
	mat4 t2 = translateMat(-this->position);
	this->_world_transform = t1 * s * t2 * this->_world_transform;
}

void MeshModel::rotateX(GLfloat theta) {
	mat4 r = rotateXMat(theta);
	mat4 t1 = translateMat(this->position);
	mat4 t2 = translateMat(-this->position);
	this->_world_transform = t1 * r * t2 * this->_world_transform;
}

void MeshModel::rotateY(GLfloat theta) {
	mat4 r = rotateYMat(theta);
	mat4 t1 = translateMat(this->position);
	mat4 t2 = translateMat(-this->position);
	this->_world_transform = t1 * r * t2 * this->_world_transform;
}

void MeshModel::rotateZ(GLfloat theta) {
	mat4 r = rotateZMat(theta);
	mat4 t1 = translateMat(this->position);
	mat4 t2 = translateMat(-this->position);
	this->_world_transform = t1 * r * t2 * this->_world_transform;
}

vector<vec4> MeshModel::getVertices() {
	return this->vertices;
}

vector<vec4> MeshModel::getVertexPositions() {
	return this->vertex_positions;
}

mat4 MeshModel::getWorldTransform() {
	return this->_world_transform;
}

vec4 MeshModel::getPosition(){ return position; }

void MeshModel::initBoundingBox(vec4 min, vec4 max) {
	//bottom face:
	this->bounding_box_vertices.push_back(vec4(min.x, min.y, min.z));
	this->bounding_box_vertices.push_back(vec4(max.x, min.y, min.z));
	this->bounding_box_vertices.push_back(vec4(max.x, max.y, min.z));
	this->bounding_box_vertices.push_back(vec4(min.x, max.y, min.z));
	//upper face:
	this->bounding_box_vertices.push_back(vec4(min.x, min.y, max.z));
	this->bounding_box_vertices.push_back(vec4(max.x, min.y, max.z));
	this->bounding_box_vertices.push_back(vec4(max.x, max.y, max.z));
	this->bounding_box_vertices.push_back(vec4(min.x, max.y, max.z));
}

void MeshModel::toggleVertices(){
	draw_pref.f_draw_vertices = !draw_pref.f_draw_vertices;
}

void MeshModel::toggleEdges(){
	draw_pref.f_draw_edges = !draw_pref.f_draw_edges;
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