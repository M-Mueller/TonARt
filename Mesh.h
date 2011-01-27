#include "GL/glew.h"

#include "ObjInterface.h"

class Mesh
{
public:
	/**
	 * Mesh takes ownership of ObjInterface
	 */
	Mesh(ObjInterface* d);
	virtual ~Mesh();

	void init();
	void draw();

private:
	GLuint vbo;
	GLuint ibo;

	ObjInterface* data;
};
