/*
 * Bass.h
 *
 *  Created on: 27.01.2011
 *      Author: keen
 */

#ifndef BASS_H_
#define BASS_H_

#include <GL/glew.h>
#include <iostream>

#include "MidiInstrument.h"
#include "Marker.h"
#include "Cube.h"
#include "Mesh.h"
#include "ObjInterface.h"

class Guitar
	: public MidiInstrument, public ObjInterface
{
public:
	Guitar(const Marker &m);
	virtual ~Guitar();

	virtual void draw();
	virtual bool isDead() const;
	virtual void isDead(bool isdead);
	
	virtual Marker getMarker();

	virtual int getVertexCount();
	virtual void getVertices(GLfloat*);
	virtual void getNormals(GLfloat*);
	virtual int getFaceCount();
	virtual void getFaces(GLuint*);
	
	virtual void startMidiOutput();
	void stopMidiOutput();
private:
	Marker position;
	Mesh* mesh;
	int lastNote;
	bool m_isDead;

	static int numGuitarVerts;
	static point3 GuitarVerts[];
	static int numGuitarFaces;
	static GLuint GuitarFaces[][3];
	static point3 CubeVertNorms[];
};

#endif /* BASS_H_ */
