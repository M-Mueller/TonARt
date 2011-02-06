/*
 * Bass.cpp
 *
 *  Created on: 27.01.2011
 *      Author: keen
 */
#ifdef _MSC_VER 
#pragma warning(disable: 4305)
#endif

#include "Guitar.h"

Guitar::Guitar(const Marker &m, const Marker& cp)
: position(m),  m_centralpoint(cp)
{
	mesh = new Mesh();
	mesh->load("guitar.obj");
}

Guitar::~Guitar()
{
	delete mesh;
}

// For a midi "note number" <--> "note name" <--> "frequency" table , see http://tonalsoft.com/pub/news/pitch-bend.aspx
void Guitar::startMidiOutput()
{
	std::vector<unsigned char> message;
	message.push_back( 0xC0 );
	message.push_back( 25 );	// Instrument
	MidiInstrument::s_midiout->sendMessage( &message );

	// Control Change: 176, 7, 100 (volume)
	message[0] = 0xB0;
	message[1] = 7;
	message.push_back( 50 );
	MidiInstrument::s_midiout->sendMessage( &message );

	// Note On: 144, 64, 90
	message[0] = 0x90;
	message[1] = MidiInstrument::s_midiNotes[getNote()];	// tonhoehe
	message[2] = 64;	// dynamik
	MidiInstrument::s_midiout->sendMessage( &message );

}

Marker Guitar::getMarker() const
{
	return position;
}

void Guitar::draw()
{
	MidiInstrument::draw();
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glLoadTransposeMatrixf(position.getTransformation());

	glScaled(0.015, 0.015, 0.015);
	glRotated(180.0, 1.0, 0.0, 0.0);

	mesh->draw();

	glPopMatrix();

}

int Guitar::getNote() const
{
	cv::Mat camToInstrument(4,4,CV_32FC1, (void*)getMarker().getTransformation());
	cv::Mat camToCenter(4,4,CV_32FC1, (void*)m_centralpoint.getTransformation());
	cv::Mat centralToInstrument = camToCenter.inv() * camToInstrument;

	return round(((int)Marker::getRotationAngleZ((float*)centralToInstrument.data))/45);
}

#ifdef _MSC_VER 
#pragma warning(default:4305)
#endif
