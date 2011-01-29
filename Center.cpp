#include "Center.h"


Center::Center(unsigned int numCircles, double ringDist) 
	: numCircles(numCircles), ringDist(ringDist), m_currentRing(0)
{
}

Center::~Center()
{

}

void Center::update(std::vector<Marker> marker)
{
	for(std::multimap<int,Instrument*>::iterator i=m_instruments.begin(); i != m_instruments.end(); i++)
	{
			delete i->second;
	}
	m_instruments.clear();

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
		if(i->getID() == 626)
			continue;

		float distance = length(i->getPosition()-centralPoint.getPosition());
		//std::cout << "Marker ID: " << i->getID() << " Distance: " << distance << std::endl;

		double safeDist = ringDist / 2.0;

		float tmp = (distance / ringDist) - 1.0f;

		Guitar* g = new Guitar((*i));
		m_instruments.insert(std::pair<int, Instrument*>((int)tmp,g));

		g->draw();

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

void Center::play()
{
	std::cout << "numIntruments: " << m_instruments.size() << std::endl;
	for(std::multimap<int,Instrument*>::iterator i=m_instruments.begin(); i != m_instruments.end(); i++)
	{
		std::cout << "instrument in ring: " << i->first << std::endl;
		std::cout << "current ring: " << m_currentRing << std::endl;
		if( i->first == (int)m_currentRing )
		{
			i->second->play();
		}
	}
	Sleep(200);


	for(std::multimap<int,Instrument*>::iterator i=m_instruments.begin(); i != m_instruments.end(); i++)
	{
		if( i->first ==(int) m_currentRing )
			i->second->stopPlaying();
	}

	m_currentRing++;

	if( m_currentRing == numCircles )
		m_currentRing = 0;
}


