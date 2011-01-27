#include "ObjInterface.h"
#include <GL/glew.h>

class Cube
	: public ObjInterface
{
public:
	Cube();
	virtual ~Cube();

	int getVertexCount(){return numCubeVerts;}
	int getFaceCount(){return numCubeFaces;}
	virtual void getVertices(GLfloat*);
	virtual void getNormals(GLfloat*);
	virtual int getFaces(GLuint*);

private:
	static int numCubeVerts;
	static point3 CubeVerts[];
	static int numCubeFaces;
	static GLuint CubeFaces[][3];
	static point3 CubeVertNorms[];
};
