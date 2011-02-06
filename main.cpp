#include <iostream>
#include <GL/glew.h>
#include <cv.h>
#include <boost/thread.hpp>

#ifdef __APPLE__
#include <glut.h>
#else
#include <GL/glut.h>
#endif

#include "Tracking.h"
#include "Center.h"

Tracking* tracking = NULL;
Center* center = NULL;

int screenWidth, screenHeight;
GLuint frameTex;

void display();
void track();
void reshape(int width, int height);
void timerFunc(int dontCareAboutRhisParam);

int main(int argc, char** argv)
{
	//init OpenGL and GLUT stuff
	glutInit(&argc, argv);
	glutInitWindowSize(640, 480);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);

	glutCreateWindow("TonARt");

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		std::cout << "GLEW init failed" <<std::endl;
	}

	glutDisplayFunc(display);
	glutIdleFunc(track);
	glutReshapeFunc(reshape);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glClearDepth(1.0);
	glClearColor(0.0, 0.5, 1.0, 1.0);

	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glGenTextures(1, &frameTex);

#ifdef __APPLE__
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4); // as 2560 is a multiple of 4
	glPixelStoref(GL_UNPACK_ROW_LENGTH, 2560 / 3); // to consider each row having this number of pixel
#endif

	glBindTexture(GL_TEXTURE_2D, frameTex);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glBindTexture(GL_TEXTURE_2D, 0);

	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	glShadeModel(GL_SMOOTH);
	
	GLfloat light0_pos[4] = {10.0f, 10.0f, -10.0f, 0.0f};
	GLfloat light0_diffuse[4] = {0.6f, 0.6f, 0.6f, 1.0f};
	GLfloat light0_ambient[4] = {0.1f, 0.1f, 0.1f, 1.0f};

	glLightfv(GL_LIGHT0, GL_POSITION, light0_pos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);

	glEnable(GL_LIGHT0);

	//init marker tracking
	tracking=new Tracking();
	center=new Center(6);

	glutTimerFunc(750,timerFunc,0);
	glutMainLoop();

	delete tracking;
	delete center;
}

void timerFunc(int dontCareAboutRhisParam)
{
	glutTimerFunc(750,timerFunc,0);

	if( center == NULL)
		return;

	boost::thread workerThread(&Center::startMidiOutput, center);  
	center->createAnimation();
	
}

void getTextureParameters(cv::Mat img, GLenum &format, GLenum &type)
{
	switch(img.type())
	{
	case CV_8UC1: format=GL_INTENSITY; type=GL_UNSIGNED_BYTE; return;
	case CV_8UC3: format=GL_BGR; type=GL_UNSIGNED_BYTE; return;
	case CV_8UC4: format=GL_BGRA; type=GL_UNSIGNED_BYTE; return;
	default: format=GL_BGR; type=GL_UNSIGNED_BYTE; return;
	}
}

void renderCamImage()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-1.0, 1.0, -1.0, 1.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	cv::Mat frame = tracking->getFrame();

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, frameTex);

	GLenum format, type;
	getTextureParameters(frame, format, type);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, frame.cols, frame.rows, 0, format, type, frame.data);

	glBegin(GL_QUADS);
	glVertex2d(-1.0, -1.0); glTexCoord2d(1.0, 1.0);
	glVertex2d(1.0, -1.0); glTexCoord2d(1.0, 0.0);
	glVertex2d(1.0, 1.0); glTexCoord2d(0.0, 0.0);
	glVertex2d(-1.0, 1.0); glTexCoord2d(0.0, 1.0);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, 0);

	glDisable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);

}

void track()
{
	std::list<Marker> markers;
	tracking->getMarkers(markers);
	center->update(markers);
	glutPostRedisplay();
}

void display()
{
	if( tracking == NULL )
		return;

	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	renderCamImage();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (float)screenWidth/(float)screenHeight, 0.01, 100.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	glClear(GL_DEPTH_BUFFER_BIT);

	center->draw();

	glutSwapBuffers();
}

void reshape(int width, int height)
{
	if(height<1)
		height=1;

	glViewport(0, 0, width, height);
	
	screenWidth=width;
	screenHeight=height;
}



