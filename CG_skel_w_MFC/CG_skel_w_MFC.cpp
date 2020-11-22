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

#define MODEL_MENU_OPEN_FILE 1
#define MODEL_MENU_ADD_PRIMITIVE 2
#define MODEL_MENU_TRANSFORM_ACTIVE_MODEL 3
#define CAMERA_MENU_ADD_CAMERA 1
#define CAMERA_MENU_TRANSFORM_ACTIVE_CAMERA 2
#define CAMERA_MENU_PROJECTION_SETTINGS 3
#define MAIN_MENU_TRANSFORM_WORLD 1
#define MAIN_MENU_PARTY 2
#define MAIN_MENU_ABOUT 3

#define SCROLL_UP 3
#define SCROLL_DOWN 4
#define TAB 9
#define DEL 127
#define SCALE 0
#define ROTATE 1
#define TRANSLATE 2
#define X 0
#define Y 1
#define Z 2
#define ORTHO 0
#define FRUSTUM 1
#define PERSPECTIVE 2

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

//===Input Functions===

void transformActiveModel() {
	if (scene->getModels().empty()) {
		cout << "There are no models </3" << endl;
		return;
	}
	int action, axis, theta;
	vec3 scale_by, translate_by;
	cout << "What do you feel like?" << endl;
	cout << "Scale: 0" << endl << "Rotate: 1" << endl << "Translate: 2" << endl;
	cin >> action;
	switch (action) {
	case SCALE:
		cout << "So... you wanna scale..." << endl;
		cout << "Enter 3 numbers for X, Y and Z scaling:" << endl;
		cin >> scale_by[0] >> scale_by[1] >> scale_by[2];
		scene->scaleSelection(scale_by);
		cout << "BEHOLD YOUR SCALED MODELS!" << endl;
		break;
	case ROTATE:
		cout << "Rotating today, are we? Choose axis:" << endl;
		cout << "X: 0" << endl << "Y: 1" << endl << "Z: 2" << endl;
		cin >> axis;
		cout << "Now - choose theta to rotate by:" << endl;
		cin >> theta;
		switch (axis) {
		case X:
			scene->rotateSelectionX(theta);
			break;
		case Y:
			scene->rotateSelectionY(theta);
			break;
		case Z:
			scene->rotateSelectionZ(theta);
			break;
		}
		cout << "BEHOLD YOUR ROTATED MODELS!" << endl;
		break;
	case TRANSLATE:
		cout << "Translation it is." << endl;
		cout << "Enter 3 numbers for X, Y and Z translation:" << endl;
		cin >> translate_by[0] >> translate_by[1] >> translate_by[2];
		scene->translateSelection(translate_by);
		cout << "BEHOLD YOUR TRANSLATED MODELS!" << endl;
		break;
	}
	cout << endl;
	scene->draw();
}

void addNewCamera() {
	cout << "Enter X, Y and Z values for your brand new GoPro Hero9 Black Edition:" << endl;
	vec4 cam_values;
	cin >> cam_values[0] >> cam_values[1] >> cam_values[2];
	Camera* cam = new Camera(cam_values);
	scene->addCamera(cam);
	cout << "Enjoy, and bring me pictures of Spiderman!" << endl;
}

void transformActiveCamera() {
	
}

void setProjectionSettings() {
	int type;
	float left, right, bottom, top, z_near, z_far, fovy, aspect;
	cout << "Choose from the following:" << endl;
	cout << "Ortho: 0" << endl << "Frustum: 1" << endl << "Perspective: 2" << endl;
	cin >> type;
	switch (type) {
	case PERSPECTIVE:
		cout << "fovy:";
		cin >> fovy;
		cout << "aspect:";
		cin >> aspect;
		cout << "z_near:";
		cin >> z_near;
		cout << "z_far:";
		cin >> z_far;
		scene->getActiveCamera()->perspective(fovy, aspect, z_near, z_far);
		break;
	default:
		cout << "left:";
		cin >> left;
		cout << "right:";
		cin >> right;
		cout << "bottom:";
		cin >> bottom;
		cout << "top:";
		cin >> top;
		cout << "z_near:";
		cin >> z_near;
		cout << "z_far:";
		cin >> z_far;
		switch (type) {
		case ORTHO:
			scene->getActiveCamera()->ortho(left, right, bottom, top, z_near, z_far);
			break;
		case FRUSTUM:
			scene->getActiveCamera()->frustum(left, right, bottom, top, z_near, z_far);
			break;
		}
	}
}

//==========

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
	case 'a':
		scene->translateSelection(vec4(-TRANSLATE_DEF, 0, 0));
		break;
	case 'D':
	case 'd':
		scene->translateSelection(vec4(TRANSLATE_DEF, 0, 0));
		break;
	case 'W':
	case 'w':
		scene->translateSelection(vec4(0, TRANSLATE_DEF, 0));
		break;
	case 'S':
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
	case 'P':
	case 'p':
		scene->toggleCameraProjection();
		break;
	case DEL:
		scene->removeModel(scene->getActiveModelIndex());
		break;
	case 'm':
	case 'M':
		scene->rotateCameraYAroundAt(ROTATE_THETA_DEF);
		break;
	case 'n':
	case 'N':
		scene->rotateCameraYAroundAt(-ROTATE_THETA_DEF);
		break;
	case 'b':
	case 'B':
		scene->rotateCameraZAroundAt(ROTATE_THETA_DEF);
		break;
	case 'v':
	case 'V':
		scene->rotateCameraZAroundAt(-ROTATE_THETA_DEF);
		break;
	case 'i':
	case 'I':
		scene->rotateCameraXAroundAt(ROTATE_THETA_DEF);
		break;
	case 'k':
	case 'K':
		scene->rotateCameraXAroundAt(-ROTATE_THETA_DEF);
		break;
	case 'f':
	case 'F':
		scene->translateCameraC(vec4(-TRANSLATE_DEF, 0,0));
		break;
	case 'h':
	case 'H':
		scene->translateCameraC(vec4(TRANSLATE_DEF, 0, 0));
		break;
	case 't':
	case 'T':
		scene->translateCameraC(vec4(0, TRANSLATE_DEF, 0));
		break;
	case 'g':
	case 'G':
		scene->translateCameraC(vec4(0, -TRANSLATE_DEF, 0));
		break;
	case 'u':
	case 'U':
		scene->zoom(1.1);
		break;
	case 'j':
	case 'J':
		scene->zoom(1/(1.1));
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




void modelsMenuCB(int id){
	CFileDialog dlg(TRUE, _T(".obj"), NULL, NULL, _T("*.obj|*.*"));
	switch (id){
	case MODEL_MENU_OPEN_FILE:
		if(dlg.DoModal()==IDOK)
		{
			std::string s((LPCTSTR)dlg.GetPathName());
			scene->loadOBJModel((LPCTSTR)dlg.GetPathName());
			scene->draw();
		}
		break;
	case MODEL_MENU_ADD_PRIMITIVE:
		scene->loadPrimModel("cube.obj");
		scene->draw();
		break;
	case MODEL_MENU_TRANSFORM_ACTIVE_MODEL:
		transformActiveModel();
		//std::cout << "MODEL_MENU_TRANSFORM_ACTIVE_MODEL" << std::endl;
		break;
	}
}

void camerasMenuCB(int id){
	switch (id){
	case CAMERA_MENU_ADD_CAMERA:
		addNewCamera();
		//std::cout << "CAMERA_MENU_ADD_CAMERA" << std::endl;
		break;
	case CAMERA_MENU_TRANSFORM_ACTIVE_CAMERA:
		transformActiveCamera();
		//std::cout << "CAMERA_MENU_TRANSFORM_ACTIVE_CAMERA" << std::endl;
		break;
	case CAMERA_MENU_PROJECTION_SETTINGS:
		setProjectionSettings();
		//std::cout << "CAMERA_MENU_PROJECTION_SETTINGS" << std::endl;
		break;
	}

	scene->draw();
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

void mainMenuCB(int id){
	switch(id){
		case MAIN_MENU_TRANSFORM_WORLD:
			std::cout << "MAIN_MENU_TRANSFORM_WORLD" << std::endl;
			break;
		case MAIN_MENU_PARTY:
			scene->party();
			break;
		case MAIN_MENU_ABOUT:
			AfxMessageBox("Never gonna give you up");
			break;
	}
}

void mainMenu(int id)
{
	//Camera* cam = NULL;
	//switch (id)
	//{
	//case MAIN_CAMERA:
	//	//TODO: change camera params
	//	//TODO: NOT WORKING
	//	cam = new Camera(vec4(10.0, 0.0, 0.0));
	//	scene->addCamera(cam);
	//	cout << scene->getActiveCameraIndex() << endl;
	//	scene->draw();
	//	break;
	//case MAIN_DEMO:
	//	scene->drawDemo();
	//	break;
	//case MAIN_PARTY:
	//	scene->party();
	//	break;
	//case MAIN_ABOUT:
	//	/*string sentences[6] = { "give you up", "let you down", "run around and desert you",
	//							"make you cry", "say goodbye", "tell a lie and hurt you" };
	//	string selected_sentence = sentences[(never_gonna++)%sentences->size()];
	//	CString strMsg;
	//	_T("%s"), (LPCTSTR)strMsg;
	//	AfxMessageBox(strMsg);*/

	//	//TODO: write manual
	//	AfxMessageBox("Never gonna give you up");
	//	break;
	//}
}

void initMenu()
{
	int modelsMenu = glutCreateMenu(modelsMenuCB);
	glutAddMenuEntry("Load Model From a File",MODEL_MENU_OPEN_FILE);
	glutAddMenuEntry("Add Primitive: Cube", MODEL_MENU_ADD_PRIMITIVE);
	glutAddMenuEntry("Transform Active Model", MODEL_MENU_TRANSFORM_ACTIVE_MODEL);
	int camerasMenu = glutCreateMenu(camerasMenuCB);
	glutAddMenuEntry("Add New Camera",CAMERA_MENU_ADD_CAMERA);
	glutAddMenuEntry("Transform Active Camera ", CAMERA_MENU_TRANSFORM_ACTIVE_CAMERA);
	glutAddMenuEntry("Projection Settings", CAMERA_MENU_PROJECTION_SETTINGS);
	glutCreateMenu(mainMenuCB);

	/*int menuCamera = glutCreateMenu(cameraMenu);
	glutAddMenuEntry("Add Camera", CAMERA_ADD);
	glutAddMenuEntry("Select Camera", CAMERA_SELECT);
	glutCreateMenu(mainMenu);*/

	glutAddSubMenu("Models",modelsMenu);
	glutAddSubMenu("Cameras",camerasMenu);
	glutAddMenuEntry("Transform World", MAIN_MENU_TRANSFORM_WORLD);
	glutAddMenuEntry("Party For 5 Seconds",MAIN_MENU_PARTY);
	glutAddMenuEntry("About",MAIN_MENU_ABOUT);
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

