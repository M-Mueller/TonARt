#ifndef INSTRUMENT_H_
#define INSTRUMENT_H_

#include <GL/gl.h>

#include "Marker.h"

class Instrument
{
public:
	Instrument(Marker &m);
	~Instrument();

	virtual void draw()=0;
	virtual void play()=0;
};

#endif
