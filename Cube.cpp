#ifdef _MSC_VER 
#pragma warning(disable : 4305)
#endif

#include "Cube.h"

int Cube::numCubeVerts = 8;
int Cube::numCubeFaces = 12;
point3 Cube::CubeVerts [] = {
		 {-0.50000, -0.50000, 0.50000},
		 {0.50000, -0.50000, 0.50000},
		 {0.50000, 0.50000, 0.50000},
		 {-0.50000, 0.50000, 0.50000},
		 {0.50000, -0.50000, -0.50000},
		 {-0.50000, -0.50000, -0.50000},
		 {-0.50000, 0.50000, -0.50000},
		 {0.50000, 0.50000, -0.50000},
	 };

GLuint Cube::CubeFaces [][3] = {
	 {0, 1, 2},
	 {0, 2, 3},
	 {4, 5, 6},
	 {4, 6, 7},
	 {5, 0, 3},
	 {5, 3, 6},
	 {1, 4, 7},
	 {1, 7, 2},
	 {5, 4, 1},
	 {5, 1, 0},
	 {3, 2, 7},
	 {3, 7, 6},
};

point3 Cube::CubeVertNorms [] = {
	 {-0.408248290463863, -0.408248290463863, 0.816496580927726},
	 {0.666666666666667, -0.666666666666667, 0.333333333333333},
	 {0.408248290463863, 0.408248290463863, 0.816496580927726},
	 {-0.666666666666667, 0.666666666666667, 0.333333333333333},
	 {0.408248290463863, -0.408248290463863, -0.816496580927726},
	 {-0.666666666666667, -0.666666666666667, -0.333333333333333},
	 {-0.408248290463863, 0.408248290463863, -0.816496580927726},
	 {0.666666666666667, 0.666666666666667, -0.333333333333333},
};

Cube::Cube()
{

}

Cube::~Cube()
{

}

void Cube::getVertices(GLfloat* v)
{
	for(int i=0; i<getVertexCount(); i++)//iterate over all vertices
	{
		for(int j=0; j<3; j++)	//iterate over all coordinates of one vertex
		{
			v[i*3+j] = CubeVerts[i][j];
		}
	}
}

void Cube::getVerticesAndNormals(GLfloat* n)
{

}

bool Cube::hasNormals()
{
	return false;
}

void Cube::getFaces(GLuint* f)
{
	for(int i=0; i<getFaceCount(); i++)//iterate over all faces
	{
		for(int j=0; j<3; j++)	//iterate over all coordinates of one face
		{
			f[i*3+j] = CubeFaces[i][j];
		}
	}
}

#ifdef _MSC_VER 
#pragma warning(default : 4305)
#endif
