#ifndef CENTER_H_
#define CENTER_H_

#include <map>
#include <iostream>
#include <vector>
#include <list>
#include <boost/thread.hpp>

#include <GL/glew.h>
#include <cv.h>

#ifdef __APPLE__
#include <glut.h>
#else
#include <GL/glut.h>
#endif

#include "Marker.h"
#include "MidiInstrument.h"
#include "Guitar.h"
#include "JumpingNote.h"

/**
 * This class represents the center from where the tones are played according to their distance to the center.
 * When update() is called, it scans the list first for the marker ID which corresponds to the center and then for
 * IDs representing an instrument. This instruments are
 * linked to the closed ring
 */
class Center
{
public:
	Center(unsigned int numCircles=4, double ringDist=0.05);
	~Center();

	void update(const std::list<Marker>& marker);

	void draw();

	// Load vertices & create vertex buffers, etc...
	void createAnimation();
	
	// runs in a different thread and has no OpenGL context
	void startMidiOutput();

private:
	Marker centralPoint;
	unsigned int numCircles;
	double ringDist;
	boost::shared_mutex instr_mutex;

	std::multimap<int, MidiInstrument*> m_instruments; //key: ring, the instrument is on
	unsigned int m_currentRing;

	std::list<JumpingNote*> m_playAnimations;

	void createJumpingNote(MidiInstrument* i);

	float rate;

	float length(cv::Vec3f);
};

#endif
