#include <GL/glew.h>
#include "ObjInterface.h"


class Cube
	: public ObjInterface
{
public:
	Cube();
	virtual ~Cube();

protected:
	int getVertexCount(){return numCubeVerts;}
	int getFaceCount(){return numCubeFaces;}
	virtual void getVertices(GLfloat*);
	virtual void getVerticesAndNormals(GLfloat*);
	bool hasNormals();
	virtual void getFaces(GLuint*);

private:
	static int numCubeVerts;
	static point3 CubeVerts[];
	static int numCubeFaces;
	static GLuint CubeFaces[][3];
	static point3 CubeVertNorms[];
};
