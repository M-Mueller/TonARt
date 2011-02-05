#ifndef MARKER_H_
#define MARKER_H_

#include <iostream>

#include "cv.h"
#include <GL/glew.h>


/**
 * Represents a marker found by the camera. The marker has an ID and a 3D position in matrix form.
 */
class Marker
{
public:
	/**
	 * Creates an empty marker with an ID of -1 and a identity matrix as position
	 */
	Marker();
	/**
	 * Creates a marker with the given ID and position. The position is copied.
	 */
	Marker(unsigned int id, float* position);
	Marker(const Marker &m);
	~Marker();

	unsigned int getID() const;
	cv::Vec3f getPosition() const;
	cv::Vec3f getRotation() const;
	cv::Vec3d Marker::getEulerAnglesXYZ() const;
	const float* getTransformation() const;

	/**
	 * Returns false if the marker has an ID of -1, true otherwise
	 */
	bool isValid();

private:
	unsigned int m_id;
	float m_position[16];
};

#endif
