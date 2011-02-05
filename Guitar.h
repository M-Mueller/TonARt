/*
 * Bass.h
 *
 *  Created on: 27.01.2011
 *      Author: keen
 */

#ifndef GUITAR_H_
#define GUITAR_H_

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
	virtual Marker getMarker() const;
	virtual int getNote() const;

	virtual bool isDead() const;
	virtual void isDead(bool isdead);
	
	virtual void startMidiOutput();
	void stopMidiOutput();

protected:
	virtual int getVertexCount();
	virtual void getVertices(GLfloat*);
	virtual void getVerticesAndNormals(GLfloat*);
	virtual int getFaceCount();
	virtual bool hasNormals();
	virtual void getFaces(GLuint*);
	
private:
	Marker position;
	Mesh* mesh;
	int lastNote;
	bool m_isDead;

	static int numGuitarVerts;
	static point3 GuitarVerts[];
	static int numGuitarFaces;
	static GLuint GuitarFaces[][3];
	static point3 GuitarVertNorms [];
};

#endif /* GUITAR_H_ */
