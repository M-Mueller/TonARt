/*
 * JumpingNote.h
 *
 *  Created on: 29.01.2011
 *      Author: keen
 */

#ifndef JUMPINGNOTE_H_
#define JUMPINGNOTE_H_

#include "cv.h"
#include <ctime>
#include <iostream>
#include "GL/glew.h"
#include "GL/glut.h"
#include "Mesh.h"


class JumpingNote
{
public:
	JumpingNote(cv::Vec3f start, cv::Vec3f direction);
	virtual ~JumpingNote();

	void draw();
	bool isDead();

private:
	cv::Vec3f currentPos;
	cv::Vec3f direction;
	float speed;
	Mesh* mesh;

	//TODO: change time measurement to milliseconds
	int liveTime;
	int maxLiveTime;
	unsigned int lastDraw;
};

#endif /* JUMPINGNOTE_H_ */
