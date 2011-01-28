#ifndef OBJINTERFACE_H_
#define OBJINTERFACE_H_

#include "graphicsDefs.h"

class ObjInterface
{
public:
	virtual int getVertexCount()=0;
	virtual void getVertices(GLfloat*)=0;
	virtual void getNormals(GLfloat*)=0;
	virtual int getFaceCount()=0;
	virtual void getFaces(GLuint*)=0;
};

#endif
