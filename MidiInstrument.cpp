#include "MidiInstrument.h"

RtMidiOut* MidiInstrument::s_midiout = new RtMidiOut("TonARt MIDI Client");
bool MidiInstrument::isMidiConnected = false;

MidiInstrument::MidiInstrument()
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

			MidiInstrument::isMidiConnected = true;
		}
	}
}