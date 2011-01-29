#include "Instrument.h"

RtMidiOut* Instrument::s_midiout = new RtMidiOut("TonARt MIDI Client");
bool Instrument::isMidiConnected = false;

Instrument::Instrument(Marker &m)
{
	if( !isMidiConnected )
	{
		// Check available ports.
		unsigned int nPorts = s_midiout->getPortCount();
		if ( nPorts == 0 ) {
			std::cout << "No ports available!\n";
			delete s_midiout;
			system("exit");
		}
		else
		{
			std::cout << nPorts << " available";

			// Open first available port.
			s_midiout->openPort( 0 );

			Instrument::isMidiConnected = true;
		}
	}
}

Instrument::~Instrument()
{

}

void Instrument::draw()
{

}

void Instrument::play()
{

}

void Instrument::stopPlaying()
{

}
