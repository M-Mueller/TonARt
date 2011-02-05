#ifndef MIDIINSTRUMENT_H_
#define MIDIINSTRUMENT_H_

#include "Marker.h"
#include <cstdlib>
#include "RtMidi.h"

class MidiInstrument{

public:
	MidiInstrument();
	virtual ~MidiInstrument(){};

	virtual void draw(){}
	virtual void startMidiOutput(){}
	virtual void stopMidiOutput(){}
	virtual Marker getMarker(){ return Marker(); }

	static RtMidiOut* s_midiout;
	static bool isMidiConnected;
	static int MidiInstrument::s_midiNotes[];
};

#endif
