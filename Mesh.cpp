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
}

void Mesh::init()
{
	GLfloat* vertices;
	unsigned int count;
	if(!data->hasNormals())
	{
		vertices = new GLfloat[data->getVertexCount()*3];
		data->getVertices(vertices);

		count = data->getVertexCount()*3*sizeof(GLfloat);
	}
	else
	{
		vertices = new GLfloat[data->getVertexCount()*3*2];
		data->getVertices(vertices);

		count = data->getVertexCount()*3*sizeof(GLfloat)*2;
	}

	//load vertex (and normal) data
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, count, vertices, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	delete vertices;

	GLuint* faces = new GLuint[data->getFaceCount()*3];
	data->getFaces(faces);

	//load indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, data->getFaceCount()*3*sizeof(GLuint), faces, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	delete faces;
}

void Mesh::draw()
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo);//vertices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);//indices

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, 0);	//vector consists of 3 floats
	if(data->hasNormals())
	{
		glEnableClientState(GL_NORMAL_ARRAY);
		glNormalPointer(GL_FLOAT, 0, (GLvoid *)(data->getVertexCount()*3*sizeof(GLfloat)));//normals start after vertices
	}

	glDrawElements(GL_TRIANGLES, data->getFaceCount()*3, GL_UNSIGNED_INT, 0);

	if(data->hasNormals())
		glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
