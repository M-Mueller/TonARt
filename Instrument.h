#ifndef INSTRUMENT_H_
#define INSTRUMENT_H_

#include <GL/glew.h>
#include <GL/glut.h>

#include "Marker.h"
#include <cstdlib>
#include "RtMidi.h"

class Instrument
{
public:
	Instrument(Marker &m);
	virtual ~Instrument();

	virtual void draw();
	virtual void play()=0;
	virtual void stopPlaying()=0;
	virtual Marker getMarker() const =0;
	virtual int getNote() const =0;

	static RtMidiOut* s_midiout;
	static bool isMidiConnected;

private:
	GLUquadric *myQuad;
	void drawGamut();
	void drawNote(int note);//the given note is drawn red, all others black
};

#endif
