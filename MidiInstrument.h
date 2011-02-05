#ifndef MIDIINSTRUMENT_H_
#define MIDIINSTRUMENT_H_

#include <GL/glew.h>
#include <GL/glut.h>

#include "Marker.h"
#include <cstdlib>
#include "RtMidi.h"

class MidiInstrument{

public:
	MidiInstrument();
	virtual ~MidiInstrument(){};

	virtual void draw();
	virtual void startMidiOutput(){}
	virtual Marker getMarker() const =0;
	virtual int getNote() const =0;

	static RtMidiOut* s_midiout;
	static bool isMidiConnected;
	static int s_midiNotes[];

private:
	GLUquadric *myQuad;
	void drawGamut();
	void drawNote(int note);//the given note is drawn red, all others black
};

#endif
