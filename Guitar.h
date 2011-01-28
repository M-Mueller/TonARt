/*
 * Bass.h
 *
 *  Created on: 27.01.2011
 *      Author: keen
 */

#ifndef BASS_H_
#define BASS_H_

#include <GL/glew.h>

#include "Instrument.h"
#include "Marker.h"
#include "Cube.h"
#include "Mesh.h"
#include "ObjInterface.h"

class Guitar
	: public Instrument, public ObjInterface
{
public:
	Guitar(Marker &m);
	virtual ~Guitar();

	virtual void draw();
	virtual void play();

	virtual int getVertexCount();
	virtual void getVertices(GLfloat*);
	virtual void getNormals(GLfloat*);
	virtual int getFaceCount();
	virtual void getFaces(GLuint*);

private:
	Marker position;
	Mesh* mesh;

	static int numGuitarVerts;
	static point3 GuitarVerts[];
	static int numGuitarFaces;
	static GLuint GuitarFaces[][3];
	static point3 CubeVertNorms[];
};

#endif /* BASS_H_ */