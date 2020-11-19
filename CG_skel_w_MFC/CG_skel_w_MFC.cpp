// CG_skel_w_MFC.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "CG_skel_w_MFC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// The one and only application object

#include "GL/glew.h"
#include "GL/freeglut.h"
#include "GL/freeglut_ext.h"
#include "vec.h"
#include "mat.h"
#include "InitShader.h"
#include "Scene.h"
#include "Renderer.h"
#include <string>

#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))

#define FILE_OPEN 1
#define MAIN_DEMO 1
#define MAIN_ABOUT 2

#define SCALE_UP_DEF 1.1
#define SCALE_DOWN_DEF (1/1.1)
#define ROTATE_THETA_DEF 5
#define TRANSLATE_DEF 0.3

Scene *scene;
Renderer *renderer;

int last_x,last_y;
bool lb_down,rb_down,mb_down;

//----------------------------------------------------------------------------
// Callbacks

void display( void )
{
}

void reshape( int width, int height )
{
	renderer->setSize(width, height);
	scene->draw();
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 033:
		exit(EXIT_SUCCESS);
		break;

	//rotate:
	case 'A':
		scene->rotateSelectionY(ROTATE_THETA_DEF);
		break;
	case 'a':
		scene->rotateSelectionY(ROTATE_THETA_DEF);
		break;
	case 'D':
		scene->rotateSelectionY(-ROTATE_THETA_DEF);
		break;
	case 'd':
		scene->rotateSelectionY(-ROTATE_THETA_DEF);
		break;
	case 'W':
		scene->rotateSelectionX(ROTATE_THETA_DEF);
		break;
	case 'w':
		scene->rotateSelectionX(ROTATE_THETA_DEF);
		break;
	case 'S':
		scene->rotateSelectionX(-ROTATE_THETA_DEF);
		break;
	case 's':
		scene->rotateSelectionX(-ROTATE_THETA_DEF);
		break;
	}
	scene->draw();
}

void mouse(int button, int state, int x, int y)
{
	//button = {GLUT_LEFT_BUTTON, GLUT_MIDDLE_BUTTON, GLUT_RIGHT_BUTTON}
	//state = {GLUT_DOWN,GLUT_UP}
	
	//set down flags
	switch(button) {
		case GLUT_LEFT_BUTTON:
			lb_down = (state==GLUT_UP)?0:1;
			break;
		case GLUT_RIGHT_BUTTON:
			rb_down = (state==GLUT_UP)?0:1;
			break;
		case GLUT_MIDDLE_BUTTON:
			mb_down = (state==GLUT_UP)?0:1;	
			break;
		case SCROLL_UP:
			//cout << "scroll up" << endl;
			scene->scaleSelection(SCALE_UP_DEF);
			break;
		case SCROLL_DOWN:
			//cout << "scroll down" << endl;
			scene->scaleSelection(SCALE_DOWN_DEF);
			break;
	}

	scene->draw();
}

void special(int key, int x, int y) {
	switch (key) {
	//translate:
	case GLUT_KEY_RIGHT:
		scene->translateSelection(vec4(TRANSLATE_DEF, 0, 0));
		break;
	case GLUT_KEY_LEFT:
		scene->translateSelection(vec4(-TRANSLATE_DEF, 0, 0));
		break;
	case GLUT_KEY_UP:
		scene->translateSelection(vec4(0, TRANSLATE_DEF, 0));
		break;
	case GLUT_KEY_DOWN:
		scene->translateSelection(vec4(0, -TRANSLATE_DEF, 0));
		break;
	
	//toggles:
	case GLUT_KEY_F1:
		scene->toggleVertices();
		break;
	case GLUT_KEY_F2:
		scene->toggleEdges();
		break;
	case GLUT_KEY_F3:
		scene->toggleBB();
		break;
	case GLUT_KEY_F4:
		scene->toggleVertexNormals();
		break;
	case GLUT_KEY_F5:
		scene->toggleFaceNormals();
		break;
	}
	

	scene->draw();
}

void motion(int x, int y)
{
	// calc difference in mouse movement
	int dx=x-last_x;
	int dy=y-last_y;
	// update last x,y
	last_x=x;
	last_y=y;

	scene->rotateSelectionY(-dx);
	scene->rotateSelectionX(-dy);
	scene->draw();
}

void fileMenu(int id)
{
	switch (id)
	{
		case FILE_OPEN:
			CFileDialog dlg(TRUE,_T(".obj"),NULL,NULL,_T("*.obj|*.*"));
			if(dlg.DoModal()==IDOK)
			{
				std::string s((LPCTSTR)dlg.GetPathName());
				scene->loadOBJModel((LPCTSTR)dlg.GetPathName());
				scene->draw();
			}
			break;
	}
}

void mainMenu(int id)
{
	switch (id)
	{
	case MAIN_DEMO:
		scene->drawDemo();
		//scene->draw();
		break;
	case MAIN_ABOUT:
		AfxMessageBox(_T("Never gonna give you up"));
		break;
	}
}

void initMenu()
{
	//TODO: add view meme
	int menuFile = glutCreateMenu(fileMenu);
	glutAddMenuEntry("Open..",FILE_OPEN);
	glutCreateMenu(mainMenu);
	glutAddSubMenu("File",menuFile);
	glutAddMenuEntry("Demo",MAIN_DEMO);
	glutAddMenuEntry("About",MAIN_ABOUT);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}
//----------------------------------------------------------------------------



int my_main( int argc, char **argv )
{
	//----------------------------------------------------------------------------
	// Initialize window
	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_RGBA| GLUT_DOUBLE);
	glutInitWindowSize( 512, 512 );
	glutInitContextVersion( 3, 2 );
	glutInitContextProfile( GLUT_CORE_PROFILE );
	glutCreateWindow( "CG" );
	glewExperimental = GL_TRUE;
	glewInit();
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		/*		...*/
	}
	fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));

	
	
	renderer = new Renderer(512,512);
	scene = new Scene(renderer);
	//----------------------------------------------------------------------------
	// Initialize Callbacks

	glutDisplayFunc( display );
	glutKeyboardFunc( keyboard );
	glutMouseFunc( mouse );
	glutSpecialFunc( special );
	glutMotionFunc ( motion );
	glutReshapeFunc( reshape );
	initMenu();
	

	glutMainLoop();
	delete scene;
	delete renderer;
	return 0;
}

CWinApp theApp;

using namespace std;

int main( int argc, char **argv )
{
	int nRetCode = 0;
	
	// initialize MFC and print and error on failure
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: change error code to suit your needs
		_tprintf(_T("Fatal Error: MFC initialization failed\n"));
		nRetCode = 1;
	}
	else
	{
		my_main(argc, argv );
	}
	
	return nRetCode;
}
