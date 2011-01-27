#ifndef CENTER_H_
#define CENTER_H_

#include <map>
#include <iostream>

#include "GL/glew.h"
#include "GL/glut.h"
#include "cv.h"

#include "Marker.h"
#include "Instrument.h"
#include "Bass.h"

class Center
{
public:
	Center();
	~Center();

	void update(std::vector<Marker> marker);

	void draw();
	void play();

private:
	Marker centralPoint;

	std::multimap<int, Instrument*> m_song; //key: ring, the instrument is on

	float rate;

	float length(cv::Vec3f);
};

#endif
