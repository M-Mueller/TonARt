#include "Center.h"


Center::Center(unsigned int numCircles, double ringDist) 
	: numCircles(numCircles), ringDist(ringDist), m_currentRing(0)
{
}

Center::~Center()
{

}

void Center::update(const std::list<Marker>& marker)
{
	instr_mutex.lock();
	for(std::multimap<int,MidiInstrument*>::iterator i=m_instruments.begin(); i != m_instruments.end(); i++)
	{
			delete i->second;
	}
	m_instruments.clear();
	instr_mutex.unlock();

	//find center
	for(std::list<Marker>::const_iterator i = marker.begin(); i != marker.end(); i++)
	{
		if(i->getID() == 626)
		{
			this->centralPoint=(*i);
		}
		else
		{
			float distance = length(i->getPosition()-centralPoint.getPosition());
			double safeDist = ringDist / 2.0;
			float tmp = (distance / ringDist) - 1.0f;

			MidiInstrument* instrument;

			switch (i->getID()) {
				case 1680:
					instrument = new Piano(*i, centralPoint);
					break;
				case 7236:
					instrument = new Guitar(*i, centralPoint);
					break;
				case 2884:
					instrument = new Drums(*i, centralPoint);
					break;
				default:
					instrument = new Piano(*i, centralPoint);
			}


			instr_mutex.lock();
			m_instruments.insert(std::pair<int, MidiInstrument*>((int)tmp,instrument));
			instr_mutex.unlock();
		}
	}

}

void Center::draw()
{
	// draw all play animations or delete them if time is up
	std::list<JumpingNote*>::iterator iter = m_playAnimations.begin();
	while (iter != m_playAnimations.end())
	{
		if ((*iter)->isDead())
		{

			delete *iter;
			m_playAnimations.erase(iter++);  // alternatively, i = items.erase(i);
		}
		else
		{
			(*iter)->draw();
			++iter;
		}
	}

	// draw instruments
	instr_mutex.lock_shared();
	for(std::multimap<int,MidiInstrument*>::iterator i=m_instruments.begin(); i != m_instruments.end(); i++)
	{
		//i->second->draw();
		i->second->draw();
	}
	instr_mutex.unlock_shared();


	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glLoadTransposeMatrixf(centralPoint.getTransformation());

	for( unsigned int i=1; i < numCircles + 1; i++ )
	{
		if(i==m_currentRing)
			glColor4d(1.0, 0.0, 0.0, 1.0);
		else
			glColor4d(1.0, 1.0, 1.0, 1.0);

		glutSolidTorus(0.001, ringDist * i, 8, 32);
	}

	glColor4d(1.0, 1.0, 1.0, 1.0);
	glPopMatrix();
}

float Center::length(cv::Vec3f v)
{
	return sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]);
}

void Center::startMidiOutput()
{
	instr_mutex.lock_shared();
	for(std::multimap<int,MidiInstrument*>::iterator i=m_instruments.begin(); i != m_instruments.end(); i++)
	{
		
		if( i->first == (int)m_currentRing )
		{
			i->second->startMidiOutput();
		}
	}
	instr_mutex.unlock_shared();

	m_currentRing++;

	if( m_currentRing == numCircles )
		m_currentRing = 0;

}

void Center::createJumpingNote(MidiInstrument* i)
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
	cv::Vec3f d(directionAR.at<float>(0,0), directionAR.at<float>(1,0), directionAR.at<float>(2,0));

	m_playAnimations.push_back(new JumpingNote(start, d));
}

void Center::createAnimation()
{
	for(std::multimap<int,MidiInstrument*>::iterator i=m_instruments.begin(); i != m_instruments.end(); i++)
	{
		if( i->first == (int)m_currentRing )
		{
			createJumpingNote(i->second);
		}
	}
}


