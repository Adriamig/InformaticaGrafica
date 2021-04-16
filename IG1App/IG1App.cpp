﻿#include "IG1App.h"
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
	mScene = new Scene;

	mCamera->set2D();
	mScene->init();

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
	delete mCamera; mCamera = nullptr;
	delete mViewPort; mViewPort = nullptr;
}
//-------------------------------------------------------------------------

void IG1App::display()
{  // double buffering

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // clears the back buffer

	if (m2Vistas) display2Vistas();
	else  display2Scenes();  // uploads the viewport and camera to the GPU

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
		mCamera->setScale(+0.01);  // zoom in  (increases the scale)
		break;
	case '-':
		mCamera->setScale(-0.01);  // zoom out (decreases the scale)
		break;
	case 'l':
		mCamera->set3D();
		break;
	case 'o':
		mCamera->set2D();
		break;
	case 'u':
		mScene->update();
		break;
	case 'U':
		animation = !animation;
		break;
	case '0':
		mScene->changeScene(0);
		break;
	case '1':
		mScene->changeScene(1);
		break;
	case 'm':
		mCamera->orbit(1, 0);
		break;
	case 'M':
		mCamera->orbit(0, 10);
		break;
	case 'p':
		mCamera->changePrj();
		break;
	case 'k':
		m2Vistas = !m2Vistas;
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
			mCamera->moveLR(1);
		//mCamera->pitch(-1);   // rotates -1 on the X axis
		else
			mCamera->moveLR(-1);
		//mCamera->pitch(1);    // rotates 1 on the X axis
		break;
	case GLUT_KEY_LEFT:
		if (mdf == GLUT_ACTIVE_CTRL)
			mCamera->moveLR(-1);
		//mCamera->yaw(1);      // rotates 1 on the Y axis 
		else
			mCamera->moveLR(1);
		//mCamera->yaw(-1);     // rotate -1 on the Y axis 
		break;
	case GLUT_KEY_UP:
		mCamera->moveUD(-1);
		//mCamera->roll(1);    // rotates 1 on the Z axis
		break;
	case GLUT_KEY_DOWN:
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
	if (animation) {
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
	Camera auxCam = *mCamera;

	Viewport auxVP = *mViewPort;

	mViewPort->setSize(mWinW / 2, mWinH);
	auxCam.setSize(mViewPort->width(), mViewPort->height());

	mViewPort->setPos(0, 0);
	mScene->changeScene(1);
	mScene->render(auxCam);
	mViewPort->setPos(mWinW / 2, 0);

	mScene->changeScene(0);
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
		mCamera->orbit(mp.x * 0.05, mp.y);
	else if (mBot == GLUT_RIGHT_BUTTON)
	{
		mCamera->moveUD(mp.y);
		mCamera->moveLR(mp.x);
	}

	glutPostRedisplay();
}

void IG1App::mouseWheel(int n, int d, int x, int y)
{
	int m = glutGetModifiers();
	if (m == 0)
		mCamera->moveFB(d);
	else
	{
		if (GLUT_ACTIVE_CTRL)
			mCamera->setScale(d);
	}

	glutPostRedisplay();
}