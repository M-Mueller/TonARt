#include "Mesh.h"

Mesh::Mesh(ObjInterface* d)
	: data(d)
{
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ibo);
}

Mesh::~Mesh()
{
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ibo);
	delete data;
}

void Mesh::init()
{
	GLfloat* vertices = new GLfloat[data->getVertexCount()*3];
	data->getVertices(vertices);

	//load vertices
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, data->getVertexCount()*3*sizeof(GLfloat), vertices, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	GLuint* faces = new GLuint[data->getFaceCount()*3];
	data->getFaces(faces);

	//load indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, data->getFaceCount()*3*sizeof(GLuint), faces, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::draw()
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo);//vertices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);//indices

	glEnableClientState(GL_VERTEX_ARRAY);
	//glEnableClientState(GL_NORMAL_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, 0);	//vector consists of 3 floats
	//glNormalPointer(GL_FLOAT, 0, (char *)(getVertices()->size()*3*sizeof(GLfloat)));//normals start after vertices

	glDrawElements(GL_TRIANGLES, data->getFaceCount()*3, GL_UNSIGNED_INT, 0);

	glDisableClientState(GL_VERTEX_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
