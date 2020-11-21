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
#define FILE_PRIMITIVE_CUBE 2
#define CAMERA_ADD 1
#define CAMERA_SELECT 2
#define MAIN_CAMERA 1
#define MAIN_DEMO 2
#define MAIN_PARTY 3
#define MAIN_ABOUT 4

#define SCROLL_UP 3
#define SCROLL_DOWN 4
#define TAB 9
#define DEL 127

#define SCALE_UP_DEF 1.1
#define SCALE_DOWN_DEF (1/1.1)
#define ROTATE_THETA_DEF 5
#define TRANSLATE_DEF 0.3

Scene *scene;
Renderer *renderer;

int last_x, last_y;
bool lb_down, rb_down, mb_down;
bool first_movement=true;
//int never_gonna = 0;

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
		scene->translateSelection(vec4(-TRANSLATE_DEF, 0, 0));
		break;
	case 'a':
		scene->translateSelection(vec4(-TRANSLATE_DEF, 0, 0));
		break;
	case 'D':
		scene->translateSelection(vec4(TRANSLATE_DEF, 0, 0));
		break;
	case 'd':
		scene->translateSelection(vec4(TRANSLATE_DEF, 0, 0));
		break;
	case 'W':
		scene->translateSelection(vec4(0, TRANSLATE_DEF, 0));
		break;
	case 'w':
		scene->translateSelection(vec4(0, TRANSLATE_DEF, 0));
		break;
	case 'S':
		scene->translateSelection(vec4(0, -TRANSLATE_DEF, 0));
		break;
	case 's':
		scene->translateSelection(vec4(0, -TRANSLATE_DEF, 0));
		break;
	case ']':
		scene->activateNextCamera();
		cout << scene->getActiveCameraIndex() << endl;
		break;
	case '[':
		scene->activatePrevCamera();
		cout << scene->getActiveCameraIndex() << endl;
		break;
	case TAB:
		scene->activateNextModel();
		break;
	case 'p':
	case 'P':
		scene->toggleCameraProjection();
		break;
	case DEL:
		scene->removeModel(scene->getActiveModelIndex());
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
			if (!lb_down) {
				first_movement = true;
			}
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
		scene->rotateSelectionY(-ROTATE_THETA_DEF);
		break;
	case GLUT_KEY_LEFT:
		scene->rotateSelectionY(ROTATE_THETA_DEF);
		break;
	case GLUT_KEY_UP:
		scene->rotateSelectionX(-ROTATE_THETA_DEF);
		break;
	case GLUT_KEY_DOWN:
		scene->rotateSelectionX(ROTATE_THETA_DEF);
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
	
	if (first_movement) {
		first_movement = false;
	}
	else {
		scene->rotateSelectionY(-dx);
		scene->rotateSelectionX(dy);
		scene->draw();
	}
}

void fileMenu(int id)
{
	CFileDialog dlg(TRUE, _T(".obj"), NULL, NULL, _T("*.obj|*.*"));
	switch (id){
	case FILE_OPEN:
		if(dlg.DoModal()==IDOK)
		{
			std::string s((LPCTSTR)dlg.GetPathName());
			scene->loadOBJModel((LPCTSTR)dlg.GetPathName());
			scene->draw();
		}
		break;
	case FILE_PRIMITIVE_CUBE:
		scene->loadPrimModel("cube.obj");
		scene->draw();
		break;
	}
}

//void cameraMenu(int id) {
//	CFileDialog dlg(TRUE, _T(".obj"), NULL, NULL, _T("*.obj|*.*"));
//	Camera* cam = NULL;
//	switch (id)
//	{
//	case CAMERA_ADD:
//		//TODO: change camera params
//		//TODO: NOT WORKING
//		cam = &Camera(vec4(5.0, 0.0, 0.0));
//		scene->addCamera(cam);
//		cout << "added camera";
//		break;
//	case CAMERA_SELECT:
//		cout << "did not implement :/" << endl;
//		break;
//	}
//}

void mainMenu(int id)
{
	Camera* cam = NULL;
	switch (id)
	{
	case MAIN_CAMERA:
		//TODO: change camera params
		//TODO: NOT WORKING
		cam = new Camera(vec4(10.0, 0.0, 0.0));
		scene->addCamera(cam);
		cout << scene->getActiveCameraIndex() << endl;
		scene->draw();
		break;
	case MAIN_DEMO:
		scene->drawDemo();
		break;
	case MAIN_PARTY:
		scene->party();
		break;
	case MAIN_ABOUT:
		/*string sentences[6] = { "give you up", "let you down", "run around and desert you",
								"make you cry", "say goodbye", "tell a lie and hurt you" };
		string selected_sentence = sentences[(never_gonna++)%sentences->size()];
		CString strMsg;
		_T("%s"), (LPCTSTR)strMsg;
		AfxMessageBox(strMsg);*/

		//TODO: write manual
		AfxMessageBox("Never gonna give you up");
		break;
	}
}

void initMenu()
{
	int menuFile = glutCreateMenu(fileMenu);
	glutAddMenuEntry("Open...",FILE_OPEN);
	glutAddMenuEntry("Add Primitive: Cube", FILE_PRIMITIVE_CUBE);
	glutCreateMenu(mainMenu);

	/*int menuCamera = glutCreateMenu(cameraMenu);
	glutAddMenuEntry("Add Camera", CAMERA_ADD);
	glutAddMenuEntry("Select Camera", CAMERA_SELECT);
	glutCreateMenu(mainMenu);*/

	glutAddSubMenu("Add Model",menuFile);
	//glutAddSubMenu("Camera", menuCamera);
	glutAddMenuEntry("Add Camera", MAIN_CAMERA);
	glutAddMenuEntry("Demo",MAIN_DEMO);
	glutAddMenuEntry("Party for 5 sec.",MAIN_PARTY);
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
	glutCreateWindow( "I AM GLUT" );
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
