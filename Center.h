#ifndef CENTER_H_
#define CENTER_H_

#include <map>
#include <iostream>

#include "GL/glew.h"
#include "GL/glut.h"
#include "cv.h"

#include "Marker.h"
#include "Instrument.h"
#include "Guitar.h"

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

	void update(std::vector<Marker> marker);

	void draw();
	void play();

private:
	Marker centralPoint;
	unsigned int numCircles;
	double ringDist;

	std::multimap<int, Instrument*> m_song; //key: ring, the instrument is on

	float rate;

	float length(cv::Vec3f);
};

#endif
