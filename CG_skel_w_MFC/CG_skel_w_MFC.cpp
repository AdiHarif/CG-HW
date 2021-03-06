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
#define SPACEBAR 32

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
//#define SCALE_DOWN_DEF (1/1.1)
#define ROTATE_THETA_DEF 5
#define TRANSLATE_DEF 0.3
#define STEP_DOWN (1/1.1)
#define STEP_UP 1.1
#define MIN_SCALE_UP 1.01
#define MAX_SCALE_DOWN 0.99

Scene *scene;
Renderer *renderer;

int last_x, last_y;
bool lb_down, rb_down, mb_down;
bool first_movement=true;
float scale_step = SCALE_UP_DEF, rotation_step = ROTATE_THETA_DEF, translation_step = TRANSLATE_DEF;
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
	cout << "How would you like to transform the active camera? (in world frame):" << endl;
	cout << "Translate: 0" << endl << "Rotate: 1" << endl;
	int trans_type;
	cin >> trans_type;
	if (trans_type == 0) {
		cout << "Enter a Translation vector (X,Y,Z)" << endl;
		float x, y, z;
		cin >> x >> y >> z;
		scene->translateCameraWorld(vec4(x, y, z, 1));
	}
	if (trans_type == 1) {
		int axis;
		cout << "Choose axis to rotate around:" << endl;
		cout << "X: 0" << endl << "Y: 1" << endl << "Z: 2" << endl;
		cin >> axis;
		cout << "Please enter an angle:" << endl;
		float theta;
		cin >> theta;

		switch (axis) {
		case X:
			scene->rotateCameraXWorld(theta);
			break;
		case Y:
			scene->rotateCameraYWorld(theta);
			break;
		case Z:
			scene->rotateCameraZWorld(theta);
			break;
		}

	}
	
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

void transformWorld() {

}

void changeAllSteps(float step_change) {
	scale_step *= step_change;
	if (step_change == STEP_DOWN)	scale_step = min(scale_step, MIN_SCALE_UP);
	else	scale_step = max(scale_step, MAX_SCALE_DOWN);
	rotation_step *= step_change;
	translation_step *= step_change;
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
		scene->translateSelection(vec4(-translation_step, 0, 0));
		break;
	case 'D':
	case 'd':
		scene->translateSelection(vec4(translation_step, 0, 0));
		break;
	case 'W':
	case 'w':
		scene->translateSelection(vec4(0, translation_step, 0));
		break;
	case 'S':
	case 's':
		scene->translateSelection(vec4(0, -translation_step, 0));
		break;
	case ']':
		scene->activateNextCamera();
		break;
	case '[':
		scene->activatePrevCamera();
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
	case 'f':
	case 'F':
		scene->translateCameraC(vec4(-translation_step, 0,0));
		break;
	case 'h':
	case 'H':
		scene->translateCameraC(vec4(translation_step, 0, 0));
		break;
	case 't':
	case 'T':
		scene->translateCameraC(vec4(0, translation_step, 0));
		break;
	case 'g':
	case 'G':
		scene->translateCameraC(vec4(0, -translation_step, 0));
		break;
	case 'u':
	case 'U':
		scene->scaleSelection(scale_step);
		break;
	case 'j':
	case 'J':
		scene->scaleSelection(1/(scale_step));
		break;
	case SPACEBAR:
		scene->lookAtActiveModel();
		break;
	case 'z':
	case 'Z':
		changeAllSteps(STEP_DOWN);
		break;
	case 'x':
	case 'X':
		changeAllSteps(STEP_UP);
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
			scene->zoom(scale_step);
			break;
		case SCROLL_DOWN:
			//cout << "scroll down" << endl;
			scene->zoom(1/scale_step);
			break;
	}

	scene->draw();
}

void special(int key, int x, int y) {
	switch (key) {
	//translate:
	case GLUT_KEY_RIGHT:
		scene->rotateSelectionY(-rotation_step);
		break;
	case GLUT_KEY_LEFT:
		scene->rotateSelectionY(rotation_step);
		break;
	case GLUT_KEY_UP:
		scene->rotateSelectionX(-rotation_step);
		break;
	case GLUT_KEY_DOWN:
		scene->rotateSelectionX(rotation_step);
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
	case GLUT_KEY_F6:
		scene->toggleCameras();
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
		scene->rotateCameraYAroundAt(-dx);
		scene->rotateCameraXAroundAt(dy);
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
			transformWorld();
			//std::cout << "MAIN_MENU_TRANSFORM_WORLD" << std::endl;
			break;
		case MAIN_MENU_PARTY:
			scene->party();
			break;
		case MAIN_MENU_ABOUT:
			/*LPCTSTR s = "asdfasdf";
			s =s+ "\n";
			s = s+ "NO WAY";
			AfxMessageBox(s);*/
			//AfxMessageBox("I don't always write long manuals, but when I do:");
			AfxMessageBox("I don't always write long manuals, but when I do:\r\n"
						"HOW TO USE:\r\n"
						"\r\nMouse:\r\n"
						"LEFT_BUTTON(hold): Move camera around lookAt point\r\n"
						"RIGHT_BUTTON: Open menu\r\n"
						"SCROLL_UP: Zoom in\r\n"
						"SCROLL_DOWN: Zoom out\r\n"

						"\r\nKeyboard:\r\n"
						"ESC: Exit program\r\n"
						"TAB: Switch between selected models\r\n"
						"W-A-S-D: Move selected models in world view\r\n"
						"]: Activate next camera(cyclic)\r\n"
						"[: Activate previous camera(cyclic)\r\n"
						"P: Toggle camera projection(Ortho-Frustum)\r\n"
						"DEL: Remove selected model\r\n"
						"F-H-T-G: Strafe selected camera\r\n"
						"U-J: Scale selected model(s)\r\n"
						"Z-X: Change step size\r\n"
						"ARROW_KEYS: Rotate selected model(s) (X, Y)\r\n"
						"F1: Toggle drawing vertices\r\n"
						"F2: Toggle drawing edges\r\n"
						"F3: Toggle drawing bounding box(es)\r\n"
						"F4: Toggle drawing vertex normals\r\n"
						"F5: Toggle drawing face normals\r\n"
						"F6: Toggle drawing cameras\r\n"

						"\r\nLegend:\r\n"
						"Green: Selected model(s)\r\n"
						"Grey: Unselected model(s)\r\n"
						"Red dot: (0, 0, 0)\r\n"
						"Yellow +: Cameras\r\n"
						"Purple: Vertex normals(of selected model(s))\r\n"
						"Red: Face normals(of selected model(s))\r\n"
						"White: Bounding box\r\n");
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
	//glutAddMenuEntry("Transform World", MAIN_MENU_TRANSFORM_WORLD);
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

