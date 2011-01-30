/*
 * JumpingNote.h
 *
 *  Created on: 29.01.2011
 *      Author: keen
 */

#ifndef JUMPINGNOTE_H_
#define JUMPINGNOTE_H_

#include "cv.h"
#include <ctime>
#include <iostream>
#include "GL/glew.h"
#include "GL/glut.h"
#include "ObjInterface.h"
#include "Mesh.h"


class JumpingNote:
	public ObjInterface
{
public:
	JumpingNote(cv::Vec3f start, cv::Vec3f direction);
	virtual ~JumpingNote();

	void draw();
	bool isDead();

	virtual int getVertexCount();
	virtual void getVertices(GLfloat* v);
	virtual void getNormals(GLfloat* n);
	virtual int getFaceCount();
	virtual void getFaces(GLuint* f);

private:
	cv::Vec3f currentPos;
	cv::Vec3f direction;
	float speed;
	Mesh* mesh;

	//TODO: change time measurement to milliseconds
	int liveTime;
	int maxLiveTime;
	unsigned int lastDraw;

	static int numNoteVerts;
	static int numNoteFaces;
	static GLuint NoteFaces [][3];
	static point3 NoteVerts [];
};

#endif /* JUMPINGNOTE_H_ */
