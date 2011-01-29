#include "Center.h"


Center::Center(unsigned int numCircles, double ringDist) : numCircles(numCircles), ringDist(ringDist)
{
}

Center::~Center()
{

}

void Center::update(std::vector<Marker> marker)
{
	//find center
	for(std::vector<Marker>::iterator i=marker.begin(); i<marker.end(); i++)
	{
		if(i->getID() == 626)
		{
			this->centralPoint=(*i);
			//marker.erase(i);
		}
		//if central marker not found->use old information
	}

	//find intruments
	for(std::vector<Marker>::iterator i=marker.begin(); i<marker.end(); i++)
	{
		int distance = length(i->getPosition()-centralPoint.getPosition());

		Guitar g((*i));

		g.draw();

//		glMatrixMode(GL_MODELVIEW);
//		glPushMatrix();
//
//		glLoadTransposeMatrixf((*i).getTransformation());
//		glLoadMatrixf(matrix);
//
//		glutSolidSphere(0.01, 8,8);
//
//		glPopMatrix();
	}
}

void Center::draw()
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glLoadTransposeMatrixf(centralPoint.getTransformation());

	for( unsigned int i=1; i < numCircles + 1; i++ ) 
		glutSolidTorus(0.001, ringDist * i, 8, 32);

	glPopMatrix();
}

float Center::length(cv::Vec3f v)
{
	return sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]);
}
