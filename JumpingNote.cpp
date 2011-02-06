/*
 * JumpingNote.cpp
 *
 *  Created on: 29.01.2011
 *      Author: keen
 */
#ifdef _MSC_VER 
#pragma warning(disable:4305)
#endif

#include "JumpingNote.h"

JumpingNote::JumpingNote(cv::Vec3f s, cv::Vec3f d)
: currentPos(s), direction(d), speed(0.02), liveTime(0), maxLiveTime(1), lastDraw(0)
{
	mesh = new Mesh();
	mesh->load("note.obj");
}

JumpingNote::~JumpingNote()
{

}

void JumpingNote::draw()
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glTranslatef(currentPos[0], currentPos[1], currentPos[2]);
	//glutSolidSphere(0.05, 8, 8);
	glScalef(0.01, 0.01, 0.01);
	mesh->draw();

	glPopMatrix();

	currentPos += (direction * speed);
	if(lastDraw>0)
	{
		liveTime+=time(NULL) - lastDraw;
	}
	lastDraw=time(NULL);
}

bool JumpingNote::isDead()
{
	return liveTime>maxLiveTime;
}

#ifdef _MSC_VER 
#pragma warning(default:4305)
#endif
