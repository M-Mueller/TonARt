/*
 * Bass.cpp
 *
 *  Created on: 27.01.2011
 *      Author: keen
 */

#include "Bass.h"

Bass::Bass(Marker &m)
: Instrument(m), position(m)
{
	Cube* c = new Cube();
	mesh = new Mesh(c);
	mesh->init();
}

Bass::~Bass()
{
	delete mesh;
}

void Bass::play()
{
}

void Bass::draw()
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glLoadTransposeMatrixf(position.getTransformation());

	glScaled(0.05, 0.05, 0.05);
	mesh->draw();

	glPopMatrix();

}

