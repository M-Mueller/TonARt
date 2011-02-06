#ifndef MESH_H_
#define MESH_H_

#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>

#include <GL/glut.h>

class Mesh
{
	struct Vertex
	{
		float x,y,z;
	};

	struct Normal
	{
		float x,y,z;
	};

	struct Face
	{
		int vertex1, vertex2, vertex3;
		int normal1, normal2, normal3;
	};

public:
	Mesh();
	virtual ~Mesh();

	bool load(std::string filename);
	void draw();

private:
	GLuint dList; //display list

	std::vector<Vertex> vertices;
	std::vector<Normal> normals;
	std::vector<Face> faces;

	void addVertex(std::string data);
	void addNormal(std::string data);
	void addFace(std::string data);
};

#endif
