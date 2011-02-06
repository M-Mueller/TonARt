/*
 * Piano.h
 *
 *  Created on: 05.02.2011
 *      Author: keen
 */

#ifndef PIANO_H_
#define PIANO_H_

#include "MidiInstrument.h"
#include "ObjInterface.h"
#include "Mesh.h"

class Piano: public MidiInstrument, public ObjInterface
{
public:
	Piano(const Marker &m);
	virtual ~Piano();

	virtual void draw(const Marker& centralpoint);
	virtual Marker getMarker() const;
	virtual int getNote() const;

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

	static int numPianoVerts;
	static point3 PianoVerts[];
	static int numPianoFaces;
	static GLuint PianoFaces[][3];
	static point3 PianoVertNorms [];
};

#endif /* PIANO_H_ */
