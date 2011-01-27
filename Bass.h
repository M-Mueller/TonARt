/*
 * Bass.h
 *
 *  Created on: 27.01.2011
 *      Author: keen
 */

#ifndef BASS_H_
#define BASS_H_

#include <GL/glew.h>

#include "Instrument.h"
#include "Marker.h"
#include "Cube.h"
#include "Mesh.h"

class Bass
	: public Instrument
{
public:
	Bass(Marker &m);
	virtual ~Bass();

	virtual void draw();
	virtual void play();

private:
	Marker position;
	Mesh* mesh;
};

#endif /* BASS_H_ */
