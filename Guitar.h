/*
 * Bass.h
 *
 *  Created on: 27.01.2011
 *      Author: keen
 */

#ifndef GUITAR_H_
#define GUITAR_H_

#include <GL/glew.h>
#include <iostream>
#include <math.h>

#include "MidiInstrument.h"
#include "Marker.h"
#include "Mesh.h"

class Guitar
	: public MidiInstrument
{
public:
	Guitar(const Marker &m);
	virtual ~Guitar();

	virtual void draw(const Marker& centralpoint);
	virtual Marker getMarker() const;
	virtual int getNote() const;
	
	virtual void startMidiOutput();
	void stopMidiOutput();
	
private:
	Marker position;
	Mesh* mesh;

	int lastNote;
	bool m_isDead;
};

#endif /* GUITAR_H_ */
