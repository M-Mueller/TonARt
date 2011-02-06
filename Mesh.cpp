/*
 * Mesh.cpp
 *
 *  Created on: 06.02.2011
 *      Author: keen
 */

#include "Mesh.h"

Mesh::Mesh()
{

}

Mesh::~Mesh()
{

}

void Mesh::draw()
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glCallList(dList);

	glPopMatrix();
}

bool Mesh::load(std::string filename)
{
	//load mesh from file system
	std::ifstream in;
	in.open(filename.c_str(), std::ios::in);
	if(!in.is_open())
		return false;

	std::string line;
	while(in.good())
	{
		//scan line by line
		std::getline(in, line);

		size_t pos = line.find(" ");
		if(pos != std::string::npos)
		{
			//each line starts with an indicator
			std::string type = line.substr(0, pos);
			if(!type.compare("#"))
			{
				continue;
			}
			else if(!type.compare("v"))//vertex
			{
				addVertex(line.substr(pos+1, line.size()));
			}
			else if(!type.compare("vn"))//vertex normal
			{
				addNormal(line.substr(pos+1, line.size()));
			}
			else if(!type.compare("f"))//face
			{
				addFace(line.substr(pos+1, line.size()));
			}
			else
			{
#ifdef DEBUG
				std::cout << "Ignored token " << type << std::endl;
#endif
			}
		}
	}

	in.close();


	//load mesh in OpenGL display list
	dList=glGenLists(1);

	glNewList(dList, GL_COMPILE);

	glBegin(GL_TRIANGLES);
	glColor4d(1.0, 1.0, 1.0, 1.0);
	std::vector<Face>::iterator iterF;
	for (iterF = faces.begin(); iterF < faces.end(); iterF++)
	{
		Vertex v1=vertices.at(iterF->vertex1-1);
		Vertex v2=vertices.at(iterF->vertex2-1);
		Vertex v3=vertices.at(iterF->vertex3-1);
		Normal n1=normals.at(iterF->normal1-1);
		Normal n2=normals.at(iterF->normal2-1);
		Normal n3=normals.at(iterF->normal3-1);

		glNormal3d(n1.x, n1.y, n1.z);
		glVertex3d(v1.x, v1.y, v1.z);
		glNormal3d(n2.x, n2.y, n2.z);
		glVertex3d(v2.x, v2.y, v2.z);
		glNormal3d(n3.x, n3.y, n3.z);
		glVertex3d(v3.x, v3.y, v3.z);
	}
	glEnd();

	glEndList();

	return true;
}

void Mesh::addVertex(std::string data)
{
	//extract the 3 float values for x,y and z from the string
	size_t posX=data.find(' ');
	size_t posY=data.find(' ', posX+1);
	size_t posZ=data.size();
	Vertex v;
	v.x=atof(data.substr(0, posX).c_str());
	v.y=atof(data.substr(posX+1, posY-posX-1).c_str());
	v.z=atof(data.substr(posY+1, posZ-posY-1).c_str());

	vertices.push_back(v);
}

void Mesh::addNormal(std::string data)
{
	//extract the 3 float values for x,y and z from the string
	size_t posX=data.find(' ');
	size_t posY=data.find(' ', posX+1);
	size_t posZ=data.size();
	Normal n;
	n.x=atof(data.substr(0, posX).c_str());
	n.y=atof(data.substr(posX+1, posY-posX-1).c_str());
	n.z=atof(data.substr(posY+1, posZ-posY-1).c_str());

	normals.push_back(n);
}

void Mesh::addFace(std::string data)
{
	Face f;

	//first find vertex and vertex attribute sections (separated by ' ')
	size_t posX=data.find(' ');
	size_t posY=data.find(' ', posX+1);
	size_t posZ=data.size();

	std::string subX=data.substr(0, posX);
	std::string subY=data.substr(posX+1, posY-posX-1);
	std::string subZ=data.substr(posY+1, posZ-posY-1);

	//find vertex, texture and normal indices in these section (each separated by '/')
	//TODO texture coordinate is currently ignored
	size_t v1_pos = subX.find('/');
	size_t t1_pos = subX.find('/', v1_pos+1);
	size_t n1_pos = subX.size();
	size_t v2_pos = subY.find('/');
	size_t t2_pos = subY.find('/', v2_pos+1);
	size_t n2_pos = subY.size();
	size_t v3_pos = subZ.find('/');
	size_t t3_pos = subZ.find('/', v3_pos+1);
	size_t n3_pos = subZ.size();

	f.vertex1 = atoi(subX.substr(0, v1_pos).c_str());
	f.normal1 = atoi(subX.substr(t1_pos+1, n1_pos-t1_pos-1).c_str());
	f.vertex2 = atoi(subY.substr(0, v2_pos).c_str());
	f.normal2 = atoi(subY.substr(t2_pos+1, n2_pos-t2_pos-1).c_str());
	f.vertex3 = atoi(subZ.substr(0, v3_pos).c_str());
	f.normal3 = atoi(subZ.substr(t3_pos+1, n3_pos-t3_pos-1).c_str());

	faces.push_back(f);
}
