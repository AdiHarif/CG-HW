#pragma once

#include "resource.h"

void display( void );
void reshape( int width, int height );
void keyboard( unsigned char key, int x, int y );
void mouse(int button, int state, int x, int y);
void special(int key, int x, int y);
void fileMenu(int id);
//void cameraMenu(int id);
void mainMenu(int id);
void initMenu();