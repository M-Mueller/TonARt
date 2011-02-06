/*
 * Drums.h
 *
 *  Created on: 06.02.2011
 *      Author: keen
 */

#ifndef DRUMS_H_
#define DRUMS_H_

#include "MidiInstrument.h"
#include "Mesh.h"

class Drums: public MidiInstrument
{
public:
	Drums(const Marker &m);
	virtual ~Drums();

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

#endif /* DRUMS_H_ */
