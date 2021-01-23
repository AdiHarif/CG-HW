#pragma once
#include "gl/glew.h"
#include "stb_image.h"
#include <string>

#define SKYBOX_FACES_COUNT 6
#define BUFF_SIZE (SKYBOX_FACES_COUNT * 3 * 6 * sizeof(float))

using namespace std;

class Skybox {
	const static float vertices[];
	const static string faces_paths[];
	int faces_count;
	
	GLuint vao;
	GLuint vbo;
	GLuint vto;
	GLuint program;

	friend class Scene;

public:
	Skybox();
	void loadCubemap();
};