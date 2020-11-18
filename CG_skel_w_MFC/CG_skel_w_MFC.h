#pragma once
#define SCROLL_UP 3
#define SCROLL_DOWN 4
#include "resource.h"

void display( void );
void reshape( int width, int height );
void keyboard( unsigned char key, int x, int y );
void mouse(int button, int state, int x, int y);
void special(int key, int x, int y);
void fileMenu(int id);
void mainMenu(int id);
void initMenu();