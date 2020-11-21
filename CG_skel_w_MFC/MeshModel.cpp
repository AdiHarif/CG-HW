#include "StdAfx.h"
#include "MeshModel.h"
#include "vec.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>


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

MeshModel::MeshModel(string fileName)
{
	loadFile(fileName);
	computeFacesNormals();
	position = vec4(0.0, 0.0, 0.0, 1.0);
	active_mesh_color = { 0.0, 1.0, 0.0 };
	inactive_mesh_color = { 0.4, 0.4, 0.4 };
	vertex_normals_color = { 1.0, 0.0, 1.0 };
	faces_normals_color = { 1.0, 0.0, 0.0 };
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
	//vector<FaceIdcs> faces;
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
		else if (lineType == "f"){ 
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
	
	
	initBoundingBox(min, max);
	//setting model position:
	position = vec4(0.0);
	//centering the model to (0,0,0):
	vec4 middle_offset = vec4((min.x + max.x) / 2, (min.y + max.y) / 2, (min.z + max.z) / 2);
	mat4 t = translateMat(-middle_offset);
	for (vector<vec4>::iterator i = vertices.begin(); i != vertices.end(); i++) {
		(*i) = t * (*i);
	}
	for (vector<vec4>::iterator i = bounding_box_vertices.begin(); i != bounding_box_vertices.end(); i++) {
		(*i) = t * (*i);
	}

	// iterate through all stored faces and create triangles
	int k=0;
	for (vector<FaceIdcs>::iterator it = faces.begin(); it != faces.end(); ++it)
	{
		for (int i = 0; i < 3; i++)
		{
			vertex_positions.push_back(vec4(vertices[it->v[i] - 1]));
			vertex_normals_indexes.push_back(it->v[i] - 1);
			vertex_normals_indexes.push_back(it->vn[i] - 1);
		}
	}

}

void MeshModel::draw(Renderer* renderer) {
	if (draw_pref.f_draw_vertices)	drawVertices(renderer);
	if (draw_pref.f_draw_edges)	drawEdges(renderer);
	if (draw_pref.f_draw_bb)	drawBoundingBox(renderer);
	if (draw_pref.f_draw_vertex_normals) drawVertexNormals(renderer);
	if (draw_pref.f_draw_faces_normals) drawFacesNormals(renderer);
}

void MeshModel::drawVertices(Renderer* renderer)
{
	Color color = is_active ? active_mesh_color : inactive_mesh_color;
	renderer->drawPoints(vertices, _world_transform, color);
}

void MeshModel::drawEdges(Renderer* renderer) {
	Color color = is_active ? active_mesh_color : inactive_mesh_color;
	renderer->drawTriangles(vertex_positions, _world_transform, color);
}

void MeshModel::drawVertexNormals(Renderer* renderer) {
	//TODO: change transformations
	/*if (vertex_normals.empty())	return;
	mat4 tcwm = tcw * _world_transform;
	vector<vec4> vecs_to_draw;
	for (int i = 0; i < vertex_normals_indexes.size(); i+=2) {
		vecs_to_draw.push_back(tcwm * vertices[vertex_normals_indexes[i]]);
		vecs_to_draw.push_back(tcwm * vertex_normals[vertex_normals_indexes[i + 1]]);
	}
	renderer->drawLines(vecs_to_draw, vertex_normals_color);*/
}

void MeshModel::drawFacesNormals(Renderer* renderer) {
	//TODO: change transformations
	/*if (faces_normals.empty())	return;
	mat4 tcwm = tcw * _world_transform;
	vector<vec4> vecs_to_draw;
	for (int i = 0; i < faces_normals.size(); i++) {
		vecs_to_draw.push_back(faces_normals_locations[i]);
		vecs_to_draw.push_back(tcwm * faces_normals[i]);
	}
	renderer->drawLines(vecs_to_draw, faces_normals_color);*/
}

void MeshModel::drawBoundingBox(Renderer* renderer) {
	vector<Edge> edges;
	//draw only vertices (can delete later)
	renderer->drawPoints(bounding_box_vertices, _world_transform, bb_color);
	//draw lines
	for (int i = 0; i < 4; i++) {//0->1, 1->2, 2->3, 3->0
		edges.push_back(Edge(bounding_box_vertices[i], bounding_box_vertices[(i + 1) % 4]));
	}
	for (int i = 0; i < 4; i++) {//4->5, 5->6, 6->7, 7->4
		edges.push_back(Edge(bounding_box_vertices[i + 4], bounding_box_vertices[((i + 1) % 4) + 4]));
	}
	for (int i = 0; i < 4; i++) {//0->4, 1->5, 2->6, 3->7
		edges.push_back(Edge(bounding_box_vertices[i], bounding_box_vertices[i + 4]));
	}
	renderer->drawLines(edges, _world_transform, bb_color);
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

void MeshModel::computeFacesNormals() {
	//TODO: transformations
	for (vector<vec4>::iterator i = vertex_positions.begin(); i != vertex_positions.end(); i+=3) {

		vec4 v0 = *(i + 1) - *(i);
		vec4 v1 = *(i + 2) - *(i);
		vec4 crs = cross(v0, v1);
		vec4 normal = crs / length(crs);
		faces_normals.push_back(normal);

		vec4 center;
		center.x = (((vec4)(*i)).x + ((vec4)(*(i + 1))).x + ((vec4)(*(i + 2))).x) / 3;
		center.y = (((vec4)(*i)).y + ((vec4)(*(i + 1))).y + ((vec4)(*(i + 2))).y) / 3;
		center.z = (((vec4)(*i)).z + ((vec4)(*(i + 1))).z + ((vec4)(*(i + 2))).z) / 3;
		faces_normals_locations.push_back(center);
	}
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

bool MeshModel::getIsActive() {
	return is_active;
}
void MeshModel::setIsActive(bool new_is_active) {
	this->is_active = new_is_active;
}