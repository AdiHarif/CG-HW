
#include "PrimMeshModel.h"

#define PRIM_FACES_COUNT 12
#define BUFF_SIZE (PRIM_FACES_COUNT * 3 * 4 * sizeof(float))


const float PrimMeshModel::vertices_buffer[] = {
	1.0, -1.0, 1.0, 1,
	-1.0, -1.0, 1.0, 1,
	-1.0, -1.0, -1.0, 1,
	-1.0, 1.0, -1.0, 1,
	-1.0, 1.0, 1.0, 1,
	1, 1.0, 1.0, 1,
	1.0, 1.0, -1, 1,
	1, 1.0, 1.0, 1,
	1.0, -1.0, 1.0, 1,
	1, 1.0, 1.0, 1,
	-1.0, 1.0, 1.0, 1,
	-1.0, -1.0, 1.0, 1,
	-1.0, -1.0, 1.0, 1,
	-1.0, 1.0, 1.0, 1,
	-1.0, 1.0, -1.0, 1,
	1.0, -1.0, -1.0, 1,
	-1.0, -1.0, -1.0, 1,
	-1.0, 1.0, -1.0, 1,
	1.0, -1.0, -1.0, 1,
	1.0, -1.0, 1.0, 1,
	-1.0, -1.0, -1.0, 1,
	1.0, 1.0, -1, 1,
	-1.0, 1.0, -1.0, 1,
	1, 1.0, 1.0, 1,
	1.0, -1.0, -1.0, 1,
	1.0, 1.0, -1, 1,
	1.0, -1.0, 1.0, 1,
	1.0, -1.0, 1.0, 1,
	1, 1.0, 1.0, 1,
	-1.0, -1.0, 1.0, 1,
	-1.0, -1.0, -1.0, 1,
	-1.0, -1.0, 1.0, 1,
	-1.0, 1.0, -1.0, 1,
	1.0, 1.0, -1, 1,
	1.0, -1.0, -1.0, 1,
	-1.0, 1.0, -1.0, 1
};

const float PrimMeshModel::normals_buffer[] = {
	-0.0, -1.0, -0.0, 1,
	-0.0, -1.0, -0.0, 1,
	-0.0, -1.0, -0.0, 1,
	0.0, 1.0, 0.0, 1,
	0.0, 1.0, 0.0, 1,
	0.0, 1.0, 0.0, 1,
	1.0, 0.0, -0.0, 1,
	1.0, 0.0, -0.0, 1,
	1.0, 0.0, -0.0, 1,
	0.0, 0.0, 1.0, 1,
	0.0, 0.0, 1.0, 1,
	0.0, 0.0, 1.0, 1,
	-1.0, 0.0, 0.0, 1,
	-1.0, 0.0, 0.0, 1,
	-1.0, 0.0, 0.0, 1,
	0.0, 0.0, -1.0, 1,
	0.0, 0.0, -1.0, 1,
	0.0, 0.0, -1.0, 1,
	-0.0, -1.0, 0.0, 1,
	-0.0, -1.0, 0.0, 1,
	-0.0, -1.0, 0.0, 1,
	0.0, 1.0, -0.0, 1,
	0.0, 1.0, -0.0, 1,
	0.0, 1.0, -0.0, 1,
	1.0, 0.0, -0.0, 1,
	1.0, 0.0, -0.0, 1,
	1.0, 0.0, -0.0, 1,
	0.0, -0.0, 1.0, 1,
	0.0, -0.0, 1.0, 1,
	0.0, -0.0, 1.0, 1,
	-1.0, 0.0, 0.0, 1,
	-1.0, 0.0, 0.0, 1,
	-1.0, 0.0, 0.0, 1,
	-0.0, -0.0, -1.0, 1,
	-0.0, -0.0, -1.0, 1,
	-0.0, -0.0, -1.0, 1
};

PrimMeshModel::PrimMeshModel(){
	faces_count = PRIM_FACES_COUNT;

	//TODO: turn buffers to vec4*
	//new, keep:
	//genVec4ArrayBuffer(BT_VERTICES, BUFF_SIZE, vertices_buffer);
	//genVec4ArrayBuffer(BT_VERTEX_NORMALS, BUFF_SIZE, normals_buffer);
	//genVec4ArrayBuffer(BT_FACE_NORMALS, BUFF_SIZE, normals_buffer);
	//----------

	//old, delete:
	//glBindBuffer(GL_ARRAY_BUFFER, vbos[BT_VERTICES]);
	//glBufferData(GL_ARRAY_BUFFER, 3*BUFF_SIZE, NULL, GL_STATIC_DRAW);
	//glBufferSubData(GL_ARRAY_BUFFER, 0, BUFF_SIZE, vertices_buffer);
	//glBufferSubData(GL_ARRAY_BUFFER, BUFF_SIZE, BUFF_SIZE, normals_buffer);
	//glBufferSubData(GL_ARRAY_BUFFER, 2*BUFF_SIZE, BUFF_SIZE, normals_buffer);
	//----------
}
