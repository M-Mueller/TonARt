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
			marker.erase(i);
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
	//draw all play animations or delete them if time is up
	for(int i=m_playAnimations.size()-1; i>=0; --i)
	{
		if(m_playAnimations.at(i)->isDead())
		{
			m_playAnimations.erase(m_playAnimations.begin()+i);
		}
		else
		{
			m_playAnimations.at(i)->draw();
		}
	}

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
			createAnimation(i->second);
		}
	}

#ifdef _WIN32
	Sleep(200);
#else
	usleep(200000);
#endif


	for(std::multimap<int,Instrument*>::iterator i=m_instruments.begin(); i != m_instruments.end(); i++)
	{
		if( i->first ==(int) m_currentRing )
			i->second->stopPlaying();
	}

	m_currentRing++;

	if( m_currentRing == numCircles )
		m_currentRing = 0;
}

void Center::createAnimation(Instrument* i)
{
	cv::Vec3f start=i->getMarker().getPosition();

	//up vector without transformation
	float directionData[] = {0.0, 0.0, -1.0, 0.0};
	cv::Mat direction(4,1,CV_32F, directionData);

	//the animation should always float in the up direction of the center
	cv::Mat transformation(4,4,CV_32F, (void*)centralPoint.getTransformation());

	//apply marker transformation to up vector (only rotation since direction.w=0)
	cv::Mat directionAR;
	cv::gemm(transformation, direction, 1.0, cv::Mat::eye(4,4,CV_32F), 0.0, directionAR);

	//convert cv::Mat to cv::Vec3f
	cv::Vec3f d(directionAR.at<float>(0,0), directionAR.at<float>(0,1), directionAR.at<float>(0,2));

	m_playAnimations.push_back(new JumpingNote(start, d));
}


