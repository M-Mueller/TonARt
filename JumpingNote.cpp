/*
 * JumpingNote.cpp
 *
 *  Created on: 29.01.2011
 *      Author: keen
 */

#include "JumpingNote.h"

JumpingNote::JumpingNote(cv::Vec3f s, cv::Vec3f d)
: currentPos(s), direction(d), speed(0.02), liveTime(0), maxLiveTime(1), lastDraw(0)
{

}

JumpingNote::~JumpingNote()
{

}

void JumpingNote::draw()
{
	glPushMatrix();

	glTranslatef(currentPos[0], currentPos[1], currentPos[2]);
	glutSolidSphere(0.05, 8, 8);

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
