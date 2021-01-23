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
#include "CDlgNewCam.h"
#include "CDlgEditCam.h"
#include "CDlgTransformModel.h"
#include "CDlgNewLight.h"
#include "CDlgEditLight.h"
#include "CDlgEditModelColor.h"
#include "NonUniformMeshModel.h"
#include "Face.h"

#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))

#define MODEL_MENU_OPEN_FILE 1
#define MODEL_MENU_OPEN_TEXTURE 2
#define MODEL_MENU_OPEN_NORMAL_MAP 3
#define MODEL_MENU_ADD_PRIMITIVE 4
#define MODEL_MENU_ADD_NON_UNIFORM 5
#define MODEL_MENU_TRANSFORM_ACTIVE_MODEL 6
#define CAMERA_MENU_ADD_CAMERA 1
#define CAMERA_MENU_EDIT_ACTIVE_CAMERA 2
#define LIGHT_MENU_ADD_LIGHT 1
#define LIGHT_MENU_EDIT_LIGHT 2
#define MAIN_MENU_TRANSFORM_WORLD 1
#define MAIN_MENU_EDIT_ACTIVE_MODEL_COLOR 2
#define MAIN_MENU_PARTY 3
#define MAIN_MENU_ABOUT 4

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
#define PARALLEL 0
#define POINT 1
#define AMBIENT 2

#define SCALE_UP_DEF 1.1
#define ROTATE_THETA_DEF 5
#define TRANSLATE_DEF 0.3
#define STEP_DOWN (1/1.1)
#define STEP_UP 1.1
#define MIN_SCALE_UP 1.01
#define MAX_SCALE_DOWN 0.99

Scene* scene;
Renderer* renderer;

int last_x, last_y;
bool lb_down, rb_down, mb_down;
bool first_movement = true;
float scale_step = SCALE_UP_DEF, rotation_step = ROTATE_THETA_DEF, translation_step = TRANSLATE_DEF;

//----------------------------------------------------------------------------

//===Input Functions===

void transformActiveModel() {
	CDlgTransformModel dlg;
	if (dlg.DoModal() == IDOK) {
		vec3 scale_vec(dlg.scale_x, dlg.scale_y, dlg.scale_z);
		vec4 translation_vec(dlg.translate_x, dlg.translate_y, dlg.translate_z);
		vec4 rotation_vec(dlg.rotate_x, dlg.rotate_y, dlg.rotate_z);

		//scale:
		if (!(scale_vec == vec3(0, 0, 0))) {
			scene->scaleSelection(scale_vec);
		}

		//translate:
		if (!(translation_vec == vec3(0, 0, 0))) {
			scene->translateSelection(translation_vec);
		}

		//rotate:
		if (!(rotation_vec == vec3(0, 0, 0)) && dlg.rot_order_index != CB_ERR) {
			switch (dlg.rot_order_index) {
			case 0://x->y->z
				scene->rotateSelectionX(rotation_vec.x);
				scene->rotateSelectionY(rotation_vec.y);
				scene->rotateSelectionZ(rotation_vec.z);
				break;
			case 1://x->z->y
				scene->rotateSelectionX(rotation_vec.x);
				scene->rotateSelectionZ(rotation_vec.z);
				scene->rotateSelectionY(rotation_vec.y);
				break;
			case 2://y->x->z
				scene->rotateSelectionY(rotation_vec.y);
				scene->rotateSelectionX(rotation_vec.x);
				scene->rotateSelectionZ(rotation_vec.z);
				break;
			case 3://y->z->x
				scene->rotateSelectionY(rotation_vec.y);
				scene->rotateSelectionZ(rotation_vec.z);
				scene->rotateSelectionX(rotation_vec.x);
				break;
			case 4://z->x->y
				scene->rotateSelectionZ(rotation_vec.z);
				scene->rotateSelectionX(rotation_vec.x);
				scene->rotateSelectionY(rotation_vec.y);
				break;
			case 5://z->y->x
				scene->rotateSelectionZ(rotation_vec.z);
				scene->rotateSelectionY(rotation_vec.y);
				scene->rotateSelectionX(rotation_vec.x);
				break;
			}
		}
	}
}

void addNewCamera() {
	CDlgNewCam dlg;
	if (dlg.DoModal() == IDOK) {
		vec4 pos = vec4(dlg.pos_x, dlg.pos_y, dlg.pos_z);
		vec4 lookat = vec4(dlg.lookat_x, dlg.lookat_y, dlg.lookat_z);
		vec4 up = vec4(dlg.up_x, dlg.up_y, dlg.up_z);
		if (!(pos == lookat)) {
			Camera* new_cam = NULL;
			if (up != vec4(0, 0, 0)) {
				new_cam = new Camera(pos, lookat);
			}
			else {
				new_cam = new Camera(pos, lookat, up);
			}
			scene->addCamera(new_cam);
		}
	}
}

void editActiveCamera() {
	CDlgEditCam dlg;
	if (dlg.DoModal() == IDOK) {
		//translate:
		vec4 translation_vec = vec4(dlg.translate_x, dlg.translate_y, dlg.translate_z);
		if (!(translation_vec == vec4(0, 0, 0))) {
			scene->translateCameraWorld(translation_vec);
		}

		//rotate:
		vec4 rotation_vec = vec4(dlg.rotate_x, dlg.rotate_y, dlg.rotate_z);
		if (!(rotation_vec == vec4(0, 0, 0)) && dlg.rot_order_index != CB_ERR) {
			switch (dlg.rot_order_index) {
			case 0://x->y->z
				scene->rotateCameraXWorld(rotation_vec.x);
				scene->rotateCameraYWorld(rotation_vec.y);
				scene->rotateCameraZWorld(rotation_vec.z);
				break;
			case 1://x->z->y
				scene->rotateCameraXWorld(rotation_vec.x);
				scene->rotateCameraZWorld(rotation_vec.z);
				scene->rotateCameraYWorld(rotation_vec.y);
				break;
			case 2://y->x->z
				scene->rotateCameraYWorld(rotation_vec.y);
				scene->rotateCameraXWorld(rotation_vec.x);
				scene->rotateCameraZWorld(rotation_vec.z);
				break;
			case 3://y->z->x
				scene->rotateCameraYWorld(rotation_vec.y);
				scene->rotateCameraZWorld(rotation_vec.z);
				scene->rotateCameraXWorld(rotation_vec.x);
				break;
			case 4://z->x->y
				scene->rotateCameraZWorld(rotation_vec.z);
				scene->rotateCameraXWorld(rotation_vec.x);
				scene->rotateCameraYWorld(rotation_vec.y);
				break;
			case 5://z->y->x
				scene->rotateCameraZWorld(rotation_vec.z);
				scene->rotateCameraYWorld(rotation_vec.y);
				scene->rotateCameraXWorld(rotation_vec.x);
				break;
			}
		}

		//projection:
		if (dlg.left != 0 || dlg.right != 0 || dlg.bottom != 0 || dlg.top != 0 || dlg.z_near != 0 || dlg.z_far != 0) {
			switch (dlg.proj_radio_index) {
			case ORTHO:
				scene->getActiveCamera()->ortho(dlg.left, dlg.right, dlg.bottom, dlg.top, dlg.z_near, dlg.z_far);
				break;
			case FRUSTUM:
				scene->getActiveCamera()->frustum(dlg.left, dlg.right, dlg.bottom, dlg.top, dlg.z_near, dlg.z_far);
				break;
			case PERSPECTIVE:
				if (dlg.z_near != 0 || dlg.z_far != 0 || dlg.fovy != 0 || dlg.aspect != 0) {
					scene->getActiveCamera()->perspective(dlg.fovy, dlg.aspect, dlg.z_near, dlg.z_far);
				}
				break;
			}
		}
	}
}

void addNewLight() {
	CDlgNewLight dlg;
	if (dlg.DoModal() == IDOK) {
		if (strcmp(dlg.name, "") == 0) {
			return;
		}
		Color c = Color(dlg.color_r, dlg.color_g, dlg.color_b);
		vec3 dir;
		vec3 pos;
		ParallelSource* parallel_s = NULL;
		PointSource* point_s = NULL;
		switch (dlg.type_radio_index) {
		case PARALLEL:
			dir = vec3(dlg.dir_x, dlg.dir_y, dlg.dir_z);
			scene->addParallelSource(ParallelSource(dlg.name, dir, c));
			break;
		case POINT:
			pos = vec3(dlg.pos_x, dlg.pos_y, dlg.pos_z);
			scene->addPointSource(PointSource(dlg.name, pos, c));
			break;
		}
	}
}

void editLight() {
	CDlgEditLight dlg;
	dlg.setParallelSources(scene->getParallelSources());
	dlg.setPointSources(scene->getPointSources());
	if (dlg.DoModal() == IDOK) {
		if (dlg.names_index < 0 && dlg.type_radio_index != 2) {
			return;
		}
		Color c = Color(dlg.color_r, dlg.color_g, dlg.color_b);
		vec3 dir;
		vec3 pos;
		ParallelSource* parallel_s = NULL;
		PointSource* point_s = NULL;
		switch (dlg.type_radio_index) {
		case PARALLEL:
			dir = vec3(dlg.dir_x, dlg.dir_y, dlg.dir_z);
			parallel_s = &scene->getParallelSources()->at(dlg.names_index);
			parallel_s->setColor(c);
			parallel_s->setDirection(dir);
			break;
		case POINT:
			pos = vec3(dlg.pos_x, dlg.pos_y, dlg.pos_z);
			point_s = &scene->getPointSources()->at(dlg.names_index);
			point_s->setColor(c);
			point_s->setPosition(pos);
			break;
		case AMBIENT:
			scene->setAmbientColor(c);
			break;
		}
	}


}


void transformWorld() {

}

void editActiveModelColor() {
	CDlgEditModelColor dlg;
	Model* m = scene->getActiveModel();
	if (!m)	return;
	dlg.setModel(m);
	if (dlg.DoModal() == IDOK) {
		Color ambient_color = Color(dlg.ambient_r, dlg.ambient_g, dlg.ambient_b);
		Color diffuse_color = Color(dlg.diffuse_r, dlg.diffuse_g, dlg.diffuse_b);
		Color specular_color = Color(dlg.specular_r, dlg.specular_g, dlg.specular_b);
		Color emit_color = Color(dlg.emit_r, dlg.emit_g, dlg.emit_b);
		m->setEmitColor(emit_color);
		m->setAmbientColor(ambient_color);
		m->setDiffuseColor(diffuse_color);
		m->setSpecularColor(specular_color);

		if (NonUniformMeshModel* non_uni_model = dynamic_cast<NonUniformMeshModel*>(m)) { // this is a non uniform model
			//do nothing
		}
		else {
			/*MeshModel* model = dynamic_cast<MeshModel*>(m);
			for (vector<Face>::iterator i = (*model->getFaces()).begin(); i != (*model->getFaces()).end(); i++) {
				i->setEmitColor(emit_color);
				i->setAmbientColor(ambient_color);
				i->setDiffuseColor(diffuse_color);
				i->setSpecularColor(specular_color);
			}*/
		}
	}
}

void changeAllSteps(float step_change) {
	scale_step *= step_change;
	if (step_change == STEP_DOWN)	scale_step = min(scale_step, MIN_SCALE_UP);
	else	scale_step = max(scale_step, MAX_SCALE_DOWN);
	rotation_step *= step_change;
	translation_step *= step_change;
}

void animateSmoothColors(int x) {
	if (!scene->getIsColorAnimationActive())	return;
	scene->updateAllModelsHSVColor();
	glutPostRedisplay();
	glutTimerFunc(20, animateSmoothColors, x);
}

void animateWaveColors(int x) {
	if (!scene->getIsColorAnimationActive())	return;
	scene->updateAllModelsWaveThreshold();
	glutPostRedisplay();
	glutTimerFunc(20, animateWaveColors, x);
}

void animateSuperNova(int x) {
	if (!scene->getIsVertexAnimationActive())	return;
	scene->updateAllModelsVertexAnimationT();
	glutPostRedisplay();
	glutTimerFunc(50, animateSuperNova, x);
}

//==========

// Callbacks

void display(void) {
	scene->draw();
}

void reshape(int width, int height)
{
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
		scene->removeSelection();
		break;
	case 'f':
	case 'F':
		scene->translateCameraC(vec4(-translation_step, 0, 0));
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
		scene->scaleSelection(1 / (scale_step));
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
	switch (button) {
	case GLUT_LEFT_BUTTON:
		lb_down = (state == GLUT_UP) ? 0 : 1;
		if (!lb_down) {
			first_movement = true;
		}
		break;
	case GLUT_RIGHT_BUTTON:
		rb_down = (state == GLUT_UP) ? 0 : 1;
		break;
	case GLUT_MIDDLE_BUTTON:
		mb_down = (state == GLUT_UP) ? 0 : 1;
		break;
	case SCROLL_UP:
		//cout << "scroll up" << endl;
		scene->zoom(scale_step);
		break;
	case SCROLL_DOWN:
		//cout << "scroll down" << endl;
		scene->zoom(1 / scale_step);
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
		scene->togglePolyMode();
		break;
	case GLUT_KEY_F2:
		scene->toggleShadingMethod();
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
	case GLUT_KEY_F7:
		scene->toggleLights();
		break;
	case GLUT_KEY_F8:
		scene->toggleAmbientMethod();
		break;
	case GLUT_KEY_F9:
		scene->toggleIsColorAnimationActive();
		if (scene->getIsColorAnimationActive()) {
			switch (scene->getActiveColorAnimationMethod()) {
			case SMOOTH:
				glutTimerFunc(20, animateSmoothColors, 0);
				break;
			case WAVE:
				glutTimerFunc(20, animateWaveColors, 0);
				break;
			}
		}
		break;
	case GLUT_KEY_F10:
		scene->toggleColorAnimationMethod();
		if (scene->getIsColorAnimationActive()) {
			switch (scene->getActiveColorAnimationMethod()) {
			case SMOOTH:
				glutTimerFunc(20, animateSmoothColors, 0);
				break;
			case WAVE:
				glutTimerFunc(20, animateWaveColors, 0);
				break;
			}
			break;
		}
		break;
	case GLUT_KEY_F11:
		scene->toggleIsVertexAnimationActive();
		if (scene->getIsVertexAnimationActive()) {
			switch (scene->getActiveVertexAnimationMethod()) {
			case SUPER_NOVA:
				glutTimerFunc(20, animateSuperNova, 0);
				break;
			}
		}
		else {
			scene->resetAllModelsVertexAnimationT();
		}
	}
	scene->draw();
}

void motion(int x, int y)
{
	// calc difference in mouse movement
	int dx = x - last_x;
	int dy = y - last_y;
	// update last x,y
	last_x = x;
	last_y = y;

	if (first_movement) {
		first_movement = false;
	}
	else {
		scene->rotateCameraYAroundAt(-dx);
		scene->rotateCameraXAroundAt(dy);
		scene->draw();
	}
}




void modelsMenuCB(int id) {
	CFileDialog dlg(TRUE, _T(".obj"), NULL, NULL, _T("*.obj|*.*"));
	switch (id) {
	case MODEL_MENU_OPEN_FILE:
		if (dlg.DoModal() == IDOK)
		{
			std::string s((LPCTSTR)dlg.GetPathName());
			scene->loadOBJModel((LPCTSTR)dlg.GetPathName());
		}
		break;
	case MODEL_MENU_OPEN_TEXTURE:
		if (dlg.DoModal() == IDOK)
		{
			std::string s((LPCTSTR)dlg.GetPathName());
			scene->loadTexture((LPCTSTR)dlg.GetPathName());
		}
		break;
	case MODEL_MENU_OPEN_NORMAL_MAP:
		if (dlg.DoModal() == IDOK)
		{
			std::string s((LPCTSTR)dlg.GetPathName());
			scene->loadNormalMap((LPCTSTR)dlg.GetPathName());
		}
		break;
	case MODEL_MENU_ADD_PRIMITIVE:
		scene->loadPrimModel();
		break;
	case MODEL_MENU_ADD_NON_UNIFORM:
		scene->loadNonUniform();
		break;
	case MODEL_MENU_TRANSFORM_ACTIVE_MODEL:
		transformActiveModel();
		break;
	}
	scene->draw();
}

void camerasMenuCB(int id) {
	switch (id) {
	case CAMERA_MENU_ADD_CAMERA:
		addNewCamera();
		break;
	case CAMERA_MENU_EDIT_ACTIVE_CAMERA:
		editActiveCamera();
		break;
	}

	scene->draw();
}

void lightsMenuCB(int id) {
	switch (id) {
	case LIGHT_MENU_ADD_LIGHT:
		addNewLight();
		break;
	case LIGHT_MENU_EDIT_LIGHT:
		editLight();
		break;
	}
	scene->draw();
}

void mainMenuCB(int id) {
	switch (id) {
	case MAIN_MENU_TRANSFORM_WORLD:
		transformWorld();
		//std::cout << "MAIN_MENU_TRANSFORM_WORLD" << std::endl;
		break;
	case MAIN_MENU_EDIT_ACTIVE_MODEL_COLOR:
		editActiveModelColor();
		break;
	case MAIN_MENU_PARTY:
		//scene->party();
		break;
	case MAIN_MENU_ABOUT:
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
	scene->draw();
}

void mainMenu(int id)
{

}

void initMenu()
{
	int modelsMenu = glutCreateMenu(modelsMenuCB);
	glutAddMenuEntry("Load Model From a File", MODEL_MENU_OPEN_FILE);
	glutAddMenuEntry("Load Texture From a File", MODEL_MENU_OPEN_TEXTURE);
	glutAddMenuEntry("Load Normal Map From a File", MODEL_MENU_OPEN_NORMAL_MAP);
	glutAddMenuEntry("Add Primitive: Cube", MODEL_MENU_ADD_PRIMITIVE);
	glutAddMenuEntry("Add Primitive: Non-Uniform Weird Dice", MODEL_MENU_ADD_NON_UNIFORM);
	glutAddMenuEntry("Transform Active Model", MODEL_MENU_TRANSFORM_ACTIVE_MODEL);
	int camerasMenu = glutCreateMenu(camerasMenuCB);
	glutAddMenuEntry("Add New Camera", CAMERA_MENU_ADD_CAMERA);
	glutAddMenuEntry("Edit Active Camera ", CAMERA_MENU_EDIT_ACTIVE_CAMERA);
	int lightsMenu = glutCreateMenu(lightsMenuCB);
	glutAddMenuEntry("Add New Light", LIGHT_MENU_ADD_LIGHT);
	glutAddMenuEntry("Edit Lights", LIGHT_MENU_EDIT_LIGHT);

	glutCreateMenu(mainMenuCB);

	glutAddSubMenu("Models", modelsMenu);
	glutAddSubMenu("Cameras", camerasMenu);
	glutAddSubMenu("Lights", lightsMenu);
	//glutAddMenuEntry("Transform World", MAIN_MENU_TRANSFORM_WORLD);
	glutAddMenuEntry("Edit Active Model Color", MAIN_MENU_EDIT_ACTIVE_MODEL_COLOR);
	glutAddMenuEntry("Party For 5 Seconds", MAIN_MENU_PARTY);
	glutAddMenuEntry("About", MAIN_MENU_ABOUT);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}
//----------------------------------------------------------------------------



int my_main(int argc, char** argv)
{
	//----------------------------------------------------------------------------
	// Initialize window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(512, 512);
	glutInitContextVersion(3, 2);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow("I AM GLUT");
	glEnable(GL_DEPTH_TEST);
	//GL_DEPTH_MASK - write/dont write to z-buffer
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

	scene = new Scene();
	//----------------------------------------------------------------------------
	// Initialize Callbacks

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutSpecialFunc(special);
	glutMotionFunc(motion);
	glutReshapeFunc(reshape);
	initMenu();

	glutMainLoop();
	delete scene;
	delete renderer;
	return 0;
}

CWinApp theApp;

using namespace std;

int main(int argc, char** argv)
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
		my_main(argc, argv);
	}

	return nRetCode;
}

