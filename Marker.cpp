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

float Marker::length(cv::Mat v) const
{
	return sqrt((float) (v.at<float>(0,0)*v.at<float>(0,0)+v.at<float>(0,1)*v.at<float>(0,1)+v.at<float>(0,2)*v.at<float>(0,2)));
}

float Marker::dot(cv::Mat v1, cv::Mat v2) const
{
	return sqrt((float) (v1.at<float>(0,0)*v2.at<float>(0,0)+v1.at<float>(0,1)*v2.at<float>(0,1)+v1.at<float>(0,2)*v2.at<float>(0,2)) );
}

float Marker::getRotationAngleZ() const
{
	//basic idea: multiply (0,1,0) with transformation matrix, project the resulting vector to the xy-plane
	//and calculate the angle between this vector and (0,1,0)

	float data[] = {0.0, 1.0, 0.0, 0.0};//up vector of the marker
	cv::Mat initial(4,1,CV_32F, data);

	//the animation should always float in the up direction of the center
	cv::Mat transformation(4,4,CV_32F, (void*)getTransformation());

	//apply marker transformation to up vector (only rotation since direction.w=0)
	cv::Mat transformed;
	cv::gemm(transformation, initial, 1.0, cv::Mat::eye(4,4,CV_32F), 0.0, transformed);
	transformed.at<float>(0,2)=0; //project to xy-plane

	float cosinus = ( transformed.at<float>(0,1) / length(transformed)); //i dot t = 0.0*t.x + 1.0*t.y + 0.0*t.z = t.y

	float angle = (acos(cosinus)*180)/M_PI;

	if(transformed.at<float>(0,0)<0)
		angle=360-angle;

	return angle;
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

