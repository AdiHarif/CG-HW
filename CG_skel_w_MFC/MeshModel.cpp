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

MeshModel::MeshModel(string fileName)
{
	loadFile(fileName);
	//computeFacesNormals();
	position = vec4(0.0, 0.0, 0.0, 1.0);
	//diffuse_color = PURPLE;
	//specular_color = WHITE;
	//ambient_color = BLUE;
	//emit_color = YELLOW;
}

MeshModel::~MeshModel(void)
{
	//delete[] vertex_positions;
}

void MeshModel::loadFile(string fileName)
{
	vec4 min, max;
	//vector<FaceIdcs> face_idcs;
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
	for (vector<Edge>::iterator i = bb_edges.begin(); i != bb_edges.end(); i++) {
		(*i) = t * (*i);
	}

	//// iterate through all stored faces and create triangles
	//for (vector<FaceIdcs>::iterator it = face_idcs.begin(); it != face_idcs.end(); ++it)
	//{
	//	Face f;
	//	for (int i = 0; i < 3; i++)
	//	{
	//		f.vertices[i] = &this->vertices[it->v[i]-1];

	//		int index = it->vn[i] - 1;
	//		if (index > -1) {
	//			f.normals[i] = &this->vertex_normals[index];
	//		}

	//		index = it->vt[i] - 1;
	//		if (index > -1) {
	//			//f.textures[i] = ?
	//		}
	//	}
	//	//TODO: add face normal calc here
	//	f.calcCenter();
	//	f.calcNormal();
	//	this->faces.push_back(f);
	//}

	for (int i = 0; i < faces.size(); i++) {

		Face f = faces[i];

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
	}
}

//void MeshModel::draw(Renderer* renderer) {
//	if (draw_pref.f_draw_vertices)	drawVertices(renderer);
//	if (draw_pref.f_draw_edges)	drawEdges(renderer);
//	if (draw_pref.f_draw_bb)	drawBoundingBox(renderer);
//	if (draw_pref.f_draw_vertex_normals) drawVertexNormals(renderer);
//	if (draw_pref.f_draw_faces_normals) drawFacesNormals(renderer);
//}
//
//void MeshModel::drawVertices(Renderer* renderer)
//{
//	Color color = is_active ? active_mesh_color : inactive_mesh_color;
//	renderer->drawVertices(vertices, tm, color);
//}
//
//void MeshModel::drawEdges(Renderer* renderer) {
//	Color color = is_active ? active_mesh_color : inactive_mesh_color;
//	renderer->drawEdges(this->faces, tm, color);
//}
//
//void MeshModel::drawVertexNormals(Renderer* renderer) {
////	//TODO: change transformations
////	/*if (vertex_normals.empty())	return;
////	mat4 tcwm = tcw * _world_transform;
////	vector<vec4> vecs_to_draw;
////	for (int i = 0; i < vertex_normals_indexes.size(); i+=2) {
////		vecs_to_draw.push_back(tcwm * vertices[vertex_normals_indexes[i]]);
////		vecs_to_draw.push_back(tcwm * vertex_normals[vertex_normals_indexes[i + 1]]);
////	}
////	renderer->drawLines(vecs_to_draw, vertex_normals_color);*/
////	/*vector<vec4> vecs_to_draw;
////	for (int i = 0; i < vertex_normals_indexes.size(); i+=2) {
////		vecs_to_draw.push_back(vertices[vertex_normals_indexes[i]]);
////		vecs_to_draw.push_back(vertex_normals[vertex_normals_indexes[i + 1]]);
////	}*/
//	if (vertex_normals.empty())	return;
//	Color color = is_active ? vertex_normals_color : inactive_mesh_color;
//	renderer->drawVertexNormals(this->faces, tm, ntm, color);
//}
//
//void MeshModel::drawFacesNormals(Renderer* renderer) {
//	//TODO: change transformations
//	/*if (faces_normals.empty())	return;
//	mat4 tcwm = tcw * _world_transform;
//	vector<vec4> vecs_to_draw;
//	for (int i = 0; i < faces_normals.size(); i++) {
//		vecs_to_draw.push_back(faces_normals_locations[i]);
//		vecs_to_draw.push_back(tcwm * faces_normals[i]);
//	}
//	renderer->drawLines(vecs_to_draw, faces_normals_color);*/
//	//if (vertex_positions.empty())	return;
//	Color color = is_active ? faces_normals_color : inactive_mesh_color;
//	renderer->drawFacesNormals(faces, tm, ntm, color);
//
//}

//void MeshModel::drawBoundingBox(Renderer* renderer) {
//	vector<Edge> edges;
//	////draw only vertices (can delete later)
//	//renderer->drawPoints(bounding_box_vertices, tm, bb_color);
//	//draw lines
//	for (int i = 0; i < 4; i++) {//0->1, 1->2, 2->3, 3->0
//		edges.push_back(Edge(bounding_box_vertices[i], bounding_box_vertices[(i + 1) % 4]));
//	}
//	for (int i = 0; i < 4; i++) {//4->5, 5->6, 6->7, 7->4
//		edges.push_back(Edge(bounding_box_vertices[i + 4], bounding_box_vertices[((i + 1) % 4) + 4]));
//	}
//	for (int i = 0; i < 4; i++) {//0->4, 1->5, 2->6, 3->7
//		edges.push_back(Edge(bounding_box_vertices[i], bounding_box_vertices[i + 4]));
//	}
//	renderer->drawEdges(edges, tm, bb_color);
//}

void MeshModel::translate(vec4 v) {
	mat4 t = translateMat(v);
	this->position = t * this->position;
	this->tm = t * this->tm;
}

void MeshModel::scale(vec3 v) {
	mat4 s = scaleMat(v);
	mat4 t1 = translateMat(this->position);
	mat4 t2 = translateMat(-this->position);
	this->tm = t1 * s * t2 * this->tm;

	// normals transformations:
	/*mat4 sn1 = scaleMat(1 / v.x, 1 / v.y, 1 / v.z);
	ntm = sn1 * ntm;*/
}

void MeshModel::rotateX(GLfloat theta) {
	mat4 r = rotateXMat(theta);
	mat4 t1 = translateMat(this->position);
	mat4 t2 = translateMat(-this->position);
	this->tm = t1 * r * t2 * this->tm;

	// normals transformations:
	ntm =  r * ntm;
}

void MeshModel::rotateY(GLfloat theta) {
	mat4 r = rotateYMat(theta);
	mat4 t1 = translateMat(this->position);
	mat4 t2 = translateMat(-this->position);
	this->tm = t1 * r * t2 * this->tm;

	// normals transformations:
	ntm =  r * ntm;
}

void MeshModel::rotateZ(GLfloat theta) {
	mat4 r = rotateZMat(theta);
	mat4 t1 = translateMat(this->position);
	mat4 t2 = translateMat(-this->position);
	this->tm = t1 * r * t2 * this->tm;

	// normals transformations:
	ntm =  r * ntm;
}

//vector<vec4> MeshModel::getVertices() {
//	return this->vertices;
//}

//vector<vec4> MeshModel::getVertexPositions() {
//	return this->vertex_positions;
//}

mat4 MeshModel::getWorldTransform() {
	return this->tm;
}

vec4 MeshModel::getPosition(){ return position; }

void MeshModel::initBoundingBox(vec4 min, vec4 max) {
	vector<Vertex> bb_vertices;

	//bottom face:
	bb_vertices.push_back(vec4(min.x, min.y, min.z));
	bb_vertices.push_back(vec4(max.x, min.y, min.z));
	bb_vertices.push_back(vec4(max.x, max.y, min.z));
	bb_vertices.push_back(vec4(min.x, max.y, min.z));
	//upper face:
	bb_vertices.push_back(vec4(min.x, min.y, max.z));
	bb_vertices.push_back(vec4(max.x, min.y, max.z));
	bb_vertices.push_back(vec4(max.x, max.y, max.z));
	bb_vertices.push_back(vec4(min.x, max.y, max.z));

	for (int i = 0; i < 4; i++) {
		bb_edges.push_back(Edge(bb_vertices[i], bb_vertices[(i + 1)%4]));
		bb_edges.push_back(Edge(bb_vertices[i+4], bb_vertices[((i+1)%4)+4]));
		bb_edges.push_back(Edge(bb_vertices[i], bb_vertices[i + 4]));
	}
}

//void MeshModel::computeFacesNormals() {
//	//TODO: transformations
//	//for (vector<vec4>::iterator i = vertex_positions.begin(); i != vertex_positions.end(); i+=3) {
//
//	//	vec4 v0 = *(i + 1) - *(i);
//	//	vec4 v1 = *(i + 2) - *(i);
//	//	vec4 crs = cross(v0, v1);
//	//	vec4 normal = crs / length(crs);
//	//	faces_normals.push_back(normal);
//
//	//	vec4 center;
//	//	center.x = (((vec4)(*i)).x + ((vec4)(*(i + 1))).x + ((vec4)(*(i + 2))).x) / 3;
//	//	center.y = (((vec4)(*i)).y + ((vec4)(*(i + 1))).y + ((vec4)(*(i + 2))).y) / 3;
//	//	center.z = (((vec4)(*i)).z + ((vec4)(*(i + 1))).z + ((vec4)(*(i + 2))).z) / 3;
//	//	faces_normals_locations.push_back(center);
//	//}
//	for (int i = 0; i < vertex_positions.size(); i+=3) {
//
//		vec4 center = vec4( (vertex_positions[i].x + vertex_positions[i + 1].x + vertex_positions[i + 2].x) / 3,
//							(vertex_positions[i].y + vertex_positions[i + 1].y + vertex_positions[i + 2].y) / 3,
//							(vertex_positions[i].z + vertex_positions[i + 1].z + vertex_positions[i + 2].z) / 3 ,
//							1.0);
//	
//		vec4 v0 = vertex_positions[i + 1] - vertex_positions[i];
//		vec4 v1 = vertex_positions[i + 2] - vertex_positions[i];
//		vec4 crs = cross(v0, v1);
//		crs.w = 0;
//		vec4 direction = crs / length(crs);
//		direction.w = 1;
//		
//		faces_to_normals.push_back(Normal(center, direction));
//	}
//}

//void MeshModel::matchNormalsToVertices() {
//	for (vector<int>::iterator i = vertex_normals_indexes.begin(); i != vertex_normals_indexes.end(); i++) {
//		
//	}
//}

//void MeshModel::toggleVertices(){
//	draw_pref.f_draw_vertices = !draw_pref.f_draw_vertices;
//}
//
//void MeshModel::toggleEdges(){
//	draw_pref.f_draw_edges = !draw_pref.f_draw_edges;
//}

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

//bool MeshModel::getIsActive() {
//	return is_active;
//}
//void MeshModel::setIsActive(bool new_is_active) {
//	this->is_active = new_is_active;
//}

void MeshModel::activate() {
	mesh_color = GREEN;
	vertex_normals_color = PURPLE;
	face_normals_color = RED;
	bb_color = WHITE;
}

void MeshModel::deactivate() {
	mesh_color = vertex_normals_color = face_normals_color = bb_color = INACTIVE_GRAY;
}
