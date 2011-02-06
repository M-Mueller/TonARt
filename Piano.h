/*
 * Piano.h
 *
 *  Created on: 05.02.2011
 *      Author: keen
 */

#ifndef PIANO_H_
#define PIANO_H_

#include "MidiInstrument.h"
#include "Mesh.h"

class Piano: public MidiInstrument
{
public:
	Piano(const Marker &m);
	virtual ~Piano();

	virtual void draw();
	virtual Marker getMarker() const;
	virtual int getNote() const;

	virtual bool isDead() const;
	virtual void isDead(bool isdead);

	virtual void startMidiOutput();
	void stopMidiOutput();

private:
	Marker position;
	Mesh* mesh;
	int lastNote;
	bool m_isDead;
};

#endif /* PIANO_H_ */
