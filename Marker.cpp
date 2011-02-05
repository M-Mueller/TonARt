#include "Marker.h"

Marker::Marker()
	: m_id(-1)
{
	for(int i=0; i<16; i++)
	{
		if(i%5==0)	//diagonal elements are 0,5,10 and 15
			m_position[i]=1.0f;
		else
			m_position[i]=0.0f;
	}
}

Marker::Marker(unsigned int id, float* position)
	: m_id(id)
{
	for(int i=0; i<16; i++)
		m_position[i]=position[i];
}

Marker::Marker(const Marker &m)
	:m_id(m.getID())
{
	for(int i=0; i<16; i++)
	{
		m_position[i]=m.getTransformation()[i];
	}
}

Marker::~Marker()
{

}

cv::Vec3f Marker::getPosition() const
{
	return cv::Vec3f(m_position[3], m_position[7], m_position[11]);
}

cv::Vec3d Marker::getEulerAnglesXYZ() const
{
	cv::Vec3d eulerXYZ;
	cv::Mat cam, rot, trans, rotx, roty, rotz;
	cv::Mat proj(3,4,CV_32FC1,(void *)m_position,0);
	cv::decomposeProjectionMatrix(proj, cam, rot, trans, rotx, roty, rotz, eulerXYZ);
	return eulerXYZ;
}

bool Marker::isValid()
{
	if(m_id!=(unsigned int)-1)
		return true;
	else
		return false;
}

unsigned int Marker::getID() const
{
	return m_id;
}

const float* Marker::getTransformation() const
{
	return m_position;
}

