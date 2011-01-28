#include "GL/glew.h"

#include "ObjInterface.h"

class Mesh
{
public:
	Mesh(ObjInterface* d);
	virtual ~Mesh();

	void init();
	void draw();

private:
	GLuint vbo;
	GLuint ibo;

	ObjInterface* data;
};
