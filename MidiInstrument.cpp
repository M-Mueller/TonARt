#include "MidiInstrument.h"

RtMidiOut* MidiInstrument::s_midiout = new RtMidiOut("TonARt MIDI Client");
bool MidiInstrument::isMidiConnected = false;
int MidiInstrument::s_midiNotes[] = {60,62,64,65,67,96,71,72};

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