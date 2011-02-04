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
	void stopPlaying();
	virtual Marker getMarker() const;
	virtual int getNote() const;

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

	static int numGuitarVerts;
	static point3 GuitarVerts[];
	static int numGuitarFaces;
	static GLuint GuitarFaces[][3];
	static point3 GuitarVertNorms [];
};

#endif /* GUITAR_H_ */
