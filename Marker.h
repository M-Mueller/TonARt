#ifndef MARKER_H_
#define MARKER_H_

#include <iostream>

#include "cv.h"
#include "GL/glew.h"

class Marker
{
public:
	Marker();
	Marker(unsigned int id, float* position);
	Marker(const Marker &m);
	~Marker();

	unsigned int getID() const;
	cv::Vec3f getPosition();
	cv::Vec3f getRotation();
	const float* getTransformation() const;
	void getTransformationGL(GLfloat* matrix);

	bool isValid();

private:
	unsigned int m_id;
	float m_position[16];
};

#endif
