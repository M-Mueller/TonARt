#ifndef OBJINTERFACE_H_
#define OBJINTERFACE_H_

#include "graphicsDefs.h"

/**
 * Abstract base class for classes which contain data that can be used by the Mesh class.
 * See Cube for an example implementation
 */
class ObjInterface
{
public:
	virtual int getVertexCount()=0;
	virtual void getVertices(GLfloat*)=0;
	virtual bool hasNormals()=0;
	virtual void getVerticesAndNormals(GLfloat*)=0;
	virtual int getFaceCount()=0;
	virtual void getFaces(GLuint*)=0;
};

#endif
