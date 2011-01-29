#ifndef INSTRUMENT_H_
#define INSTRUMENT_H_

#include "Marker.h"
#include <cstdlib>
#include "RtMidi.h"

class Instrument
{
public:
	Instrument(Marker &m);
	virtual ~Instrument();

	virtual void draw();
	virtual void play();
	virtual void stopPlaying();
	virtual Marker getMarker()=0;

	static RtMidiOut* s_midiout;
	static bool isMidiConnected;
};

#endif
