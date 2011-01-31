#ifndef MESH_H_
#define MESH_H_

#include "GL/glew.h"
#include <iostream>

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

#endif
