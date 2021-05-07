#include "IG1App.h"
#include "CheckML.h"
#include <iostream>

using namespace std;

//-------------------------------------------------------------------------
// static single instance (singleton pattern)

IG1App IG1App::s_ig1app;  // default constructor (constructor with no parameters)

//-------------------------------------------------------------------------

void IG1App::close()
{
	if (!mStop) {  // if main loop has not stopped
		cout << "Closing glut...\n";
		glutLeaveMainLoop();  // stops main loop and destroy the OpenGL context
		mStop = true;   // main loop stopped  
	}
	free();
}
//-------------------------------------------------------------------------

void IG1App::run()   // enters the main event processing loop
{
	if (mWinId == 0) { // if not initialized
		init();       // initialize the application 
		glutMainLoop();      // enters the main event processing loop 
		mStop = true;  // main loop has stopped  
	}
}
//-------------------------------------------------------------------------

void IG1App::init()
{
	// create an OpenGL Context
	iniWinOpenGL();

	// create the scene after creating the context 
	// allocate memory and resources
	mViewPort = new Viewport(mWinW, mWinH); //glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT)
	mCamera = new Camera(mViewPort);
	mCamera2 = new Camera(mViewPort);
	mScene = new Scene;
	mScene2 = new Scene;

	mCamera->set2D();
	mCamera2->set2D();
	mScene->init();
	mScene2->changeScene(1);
	mScene2->init();

}
//-------------------------------------------------------------------------

void IG1App::iniWinOpenGL()
{  // Initialization
	cout << "Starting glut...\n";
	int argc = 0;
	glutInit(&argc, nullptr);

	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);  // GLUT_CORE_PROFILE
	glutInitContextFlags(GLUT_DEBUG);		// GLUT_FORWARD_COMPATIBLE

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	glutInitWindowSize(mWinW, mWinH);   // window size
	//glutInitWindowPosition (140, 140);

	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH /*| GLUT_STENCIL*/); // RGBA colors, double buffer, depth buffer and stencil buffer   

	mWinId = glutCreateWindow("IG1App");  // with its associated OpenGL context, return window's identifier 

	// Callback registration
	glutReshapeFunc(s_resize);
	glutKeyboardFunc(s_key);
	glutSpecialFunc(s_specialKey);
	glutDisplayFunc(s_display);
	glutIdleFunc(s_update);
	glutMouseFunc(s_mouse);
	glutMotionFunc(s_motion);
	glutMouseWheelFunc(s_mouseWheel);

	cout << glGetString(GL_VERSION) << '\n';
	cout << glGetString(GL_VENDOR) << '\n';
}
//-------------------------------------------------------------------------

void IG1App::free()
{  // release memory and resources
	delete mScene; mScene = nullptr;
	delete mScene2; mScene2 = nullptr;
	delete mCamera; mCamera = nullptr;
	delete mCamera2; mCamera2 = nullptr;
	delete mViewPort; mViewPort = nullptr;
}
//-------------------------------------------------------------------------

void IG1App::display()
{  // double buffering

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // clears the back buffer

	if (m2Vistas) display2Vistas();
	else if (m2Scenes) display2Scenes();
	mScene->render(*mCamera);  // uploads the viewport and camera to the GPU

	glutSwapBuffers();	// swaps the front and back buffer
}
//-------------------------------------------------------------------------

void IG1App::resize(int newWidth, int newHeight)
{
	mWinW = newWidth; mWinH = newHeight;

	// Resize Viewport to the new window size
	mViewPort->setSize(newWidth, newHeight);

	// Resize Scene Visible Area such that the scale is not modified
	mCamera->setSize(mViewPort->width(), mViewPort->height());
}
//-------------------------------------------------------------------------

void IG1App::key(unsigned char key, int x, int y)
{
	bool need_redisplay = true;

	switch (key) {
	case 27:  // Escape key 
		glutLeaveMainLoop();  // stops main loop and destroy the OpenGL context
	case '+':
		if (m2Scenes && x < mWinW / 2)
			mCamera2->setScale(+0.01);  // zoom in  (increases the scale)
		else
			mCamera->setScale(+0.01);  // zoom in  (increases the scale)
		break;
	case '-':
		if (m2Scenes && x < mWinW / 2)
			mCamera2->setScale(-0.01);  // zoom out (decreases the scale)
		else
			mCamera->setScale(-0.01);  // zoom out (decreases the scale)
		break;
	case 'l':
		if (m2Scenes && x < mWinW / 2)
			mCamera2->set3D();
		else
			mCamera->set3D();
		break;
	case 'o':
		if (m2Scenes && x < mWinW / 2)
			mCamera2->set2D();
		else
			mCamera->set2D();
		break;
	case 'u':
		if (m2Scenes && x < mWinW / 2)
			mScene2->update();
		else
			mScene->update();
		break;
	case 'U':
		if (m2Scenes && x < mWinW / 2)
			animation2 = !animation2;
		else
			animation = !animation;
		break;
	case '0':
		mScene->changeScene(0);
		break;
	case '1':
		if (!m2Scenes)
			mScene->changeScene(1);
		break;
	case '2':
		if (!m2Scenes)
			mScene->changeScene(2);
		break;
	case 'm':
		if (m2Scenes && x < mWinW / 2)
			mCamera2->orbit(1, 1);
		else
			mCamera->orbit(1, 1);
		break;
	case 'p':
		if (m2Scenes && x < mWinW / 2)
			mCamera2->changePrj();
		else
			mCamera->changePrj();
		break;
	case 'k':
		if (m2Scenes) m2Scenes = false;
		m2Vistas = !m2Vistas;
		break;
	case 't':
		if (m2Vistas) m2Vistas = false;
		m2Scenes = !m2Scenes;
		mScene->changeScene(0);
		break;
	default:
		need_redisplay = false;
		break;
	} //switch

	if (need_redisplay)
		glutPostRedisplay(); // marks the window as needing to be redisplayed -> calls to display()
}
//-------------------------------------------------------------------------

void IG1App::specialKey(int key, int x, int y)
{
	bool need_redisplay = true;
	int mdf = glutGetModifiers(); // returns the modifiers (Shift, Ctrl, Alt)

	switch (key) {
	case GLUT_KEY_RIGHT:
		if (mdf == GLUT_ACTIVE_CTRL)
			if (m2Scenes && x < mWinW / 2)
				mCamera2->moveLR(1);
			else
				mCamera->moveLR(1);
		//mCamera->pitch(-1);   // rotates -1 on the X axis
		else
			if (m2Scenes && x < mWinW / 2)
				mCamera2->moveLR(-1);
			else
				mCamera->moveLR(-1);
		//mCamera->pitch(1);    // rotates 1 on the X axis
		break;
	case GLUT_KEY_LEFT:
		if (mdf == GLUT_ACTIVE_CTRL)
			if (m2Scenes && x < mWinW / 2)
				mCamera2->moveLR(-1);
			else
				mCamera->moveLR(-1);
		//mCamera->yaw(1);      // rotates 1 on the Y axis 
		else
			if (m2Scenes && x < mWinW / 2)
				mCamera2->moveLR(1);
			else
				mCamera->moveLR(1);
		//mCamera->yaw(-1);     // rotate -1 on the Y axis 
		break;
	case GLUT_KEY_UP:
		if (m2Scenes && x < mWinW / 2)
			mCamera2->moveUD(-1);
		else
			mCamera->moveUD(-1);
		//mCamera->roll(1);    // rotates 1 on the Z axis
		break;
	case GLUT_KEY_DOWN:
		if (m2Scenes && x < mWinW / 2)
			mCamera2->moveUD(1);
		else
			mCamera->moveUD(1);
		//mCamera->roll(-1);   // rotates -1 on the Z axis
		break;
	default:
		need_redisplay = false;
		break;
	}//switch

	if (need_redisplay)
		glutPostRedisplay(); // marks the window as needing to be redisplayed -> calls to display()
}
//-------------------------------------------------------------------------

void IG1App::update() {
	if (m2Scenes && animation2) {
		GLuint mLastUpdateTime = 0;
		int timer = glutGet(GLUT_ELAPSED_TIME);
		if ((timer - mLastUpdateTime) >= 1000 / 60) {
			if (animation)
				mScene->update();
			mScene2->update();
			mLastUpdateTime = timer;
			glutPostRedisplay();
		}
	}
	else if (animation) {
		GLuint mLastUpdateTime = 0;
		int timer = glutGet(GLUT_ELAPSED_TIME);
		if ((timer - mLastUpdateTime) >= 1000 / 60) {
			mScene->update();
			mLastUpdateTime = timer;
			glutPostRedisplay();
		}
	}
}

void IG1App::display2Vistas()
{
	Camera auxCam = *mCamera;

	Viewport auxVP = *mViewPort;

	mViewPort->setSize(mWinW / 2, mWinH);
	auxCam.setSize(mViewPort->width(), mViewPort->height());

	mViewPort->setPos(0, 0);
	mScene->render(auxCam);
	mViewPort->setPos(mWinW / 2, 0);

	auxCam.setCenital();
	mScene->render(auxCam);
	*mViewPort = auxVP;
}

void IG1App::display2Scenes()
{

	Viewport auxVP = *mViewPort;

	mViewPort->setSize(mWinW / 2, mWinH);

	Camera auxCam = *mCamera2;
	auxCam.setSize(mViewPort->width(), mViewPort->height());
	mViewPort->setPos(0, 0);
	mScene2->render(auxCam);

	auxCam = *mCamera;
	auxCam.setSize(mViewPort->width(), mViewPort->height());
	mViewPort->setPos(mWinW / 2, 0);
	mScene->render(auxCam);

	*mViewPort = auxVP;
}


void IG1App::mouse(int button, int state, int x, int y)
{
	//y = glutGet(GLUT_WINDOW_HEIGHT) - y;
	mBot = button;
	mCoord.x = x;
	mCoord.y = y;
}

void IG1App::motion(int x, int y)
{
	glm::dvec2 mp;
	mp.x = mCoord.x - x;
	mp.y = mCoord.y - y;

	mCoord.x = x;
	mCoord.y = y;

	if (mBot == GLUT_LEFT_BUTTON)
		if (m2Scenes && x < mWinW / 2)
			mCamera2->orbit(mp.x * 0.05, mp.y);
		else
			mCamera->orbit(mp.x * 0.05, mp.y);
	else if (mBot == GLUT_RIGHT_BUTTON)
	{
		if (m2Scenes && x < mWinW / 2) {
			mCamera2->moveUD(mp.y);
			mCamera2->moveLR(mp.x);
		}
		else {
			mCamera->moveUD(mp.y);
			mCamera->moveLR(mp.x);
		}
	}

	glutPostRedisplay();
}

void IG1App::mouseWheel(int n, int d, int x, int y)
{
	int m = glutGetModifiers();
	if (m == 0)
		if (m2Scenes && x < mWinW / 2)
			mCamera2->moveFB(d);
		else
			mCamera->moveFB(d);
	else
	{
		if (GLUT_ACTIVE_CTRL)
			if (m2Scenes && x < mWinW / 2)
				mCamera2->setScale(d);
			else
				mCamera->setScale(d);
	}

	glutPostRedisplay();
}