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
#include "Instrument.h"
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

	void update(std::list<Marker> marker);

	void draw();
	void play();

private:
	Marker centralPoint;
	unsigned int numCircles;
	double ringDist;
	boost::mutex instr_mutex;

	std::multimap<int, Instrument*> m_instruments; //key: ring, the instrument is on
	unsigned int m_currentRing;

	std::vector<JumpingNote*> m_playAnimations;
	void createAnimation(Instrument* i);

	float rate;

	float length(cv::Vec3f);
};

#endif
