#ifndef INSTRUMENT_H_
#define INSTRUMENT_H_

#include "Marker.h"

class Instrument
{
public:
	Instrument(Marker &m);
	virtual ~Instrument();

	virtual void draw()=0;
	virtual void play()=0;
};

#endif
