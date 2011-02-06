#include "MidiInstrument.h"

RtMidiOut* MidiInstrument::s_midiout = new RtMidiOut("TonARt MIDI Client");
bool MidiInstrument::isMidiConnected = false;
int MidiInstrument::s_midiNotes[] = {60,62,64,65,67,69,71,72};

MidiInstrument::MidiInstrument()
{
	myQuad = gluNewQuadric();

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

void MidiInstrument::draw(const Marker& centralpoint)
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	cv::Mat camToInstrument(4,4,CV_32FC1, (void*)getMarker().getTransformation());
	cv::Mat camToCenter(4,4,CV_32FC1, (void*)centralpoint.getTransformation());
	cv::Mat centralToInstrument = camToCenter.inv() * camToInstrument;

	//glLoadTransposeMatrixf((float*)centralToInstrument.data);
	glLoadTransposeMatrixf((float*)getMarker().getTransformation());

	glScalef(0.004, 0.004, 0.004);
	drawGamut();
	drawNote(getNote());

	glPopMatrix();
	glColor4d(1.0, 1.0, 1.0, 1.0);//reset color
}

void MidiInstrument::drawGamut()
{
	float laengeZeile = 30.0;

	glRotatef(90.0, 0.0, 1.0, 0.0);
	glRotatef(getMarker().getRotationAngleZ(), 1, 0, 0);//always parallel to camera

	glTranslated(0.0, -15.0, -laengeZeile/2.0);

	glColor3f(0,0,0);

	for(int i=0; i<=4; i++)
	{
		gluCylinder(myQuad, 0.07, 0.07, laengeZeile, 25, 20);
		glTranslatef(0,1,0);
	}

	//Anfang+Ende von Zeilen
	glPushMatrix();
	glRotatef(90, 1,0,0);
	glTranslatef(0,0,1);
	gluCylinder(myQuad, 0.15, 0.15, 4, 20, 20);
	glTranslatef(0, laengeZeile, 0);
	gluCylinder(myQuad, 0.15, 0.15, 4, 20, 20);
	glPopMatrix();
}

void MidiInstrument::drawNote(int note)
{
	glTranslatef(0,-2.5, 3);

	for(int i=0; i<=7; i++)
	{

		// Passende Note zur aktuellen Tonlage soll rot
		if (i == note){
			glColor3f(1,0,0);
		} else {
			glColor3f(0,0,0);
		}

		// Notenkopf
		glPushMatrix();
		glScalef(1,1,1.4);
		glutSolidSphere(0.45,10,10);
		glPopMatrix();

		// Notenhals
		glPushMatrix();
		glRotatef(90, 1,0,0);

		if(i<=1){
			glTranslatef(0, 0.55,0);
		} else {
			glTranslatef(0,-0.55,-2.8);
		}

		gluCylinder(myQuad, 0.08, 0.08, 3, 20, 20);
		glPopMatrix();

		// Strich für untere C-Note
		if(i == 7){
			glPushMatrix();
			glTranslatef(0,0,-1);
			gluCylinder(myQuad, 0.05, 0.05, 2, 20, 20);
			glPopMatrix();
		}

		glTranslatef(0,-0.5, 3.5);
	}
}
