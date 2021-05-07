#include "IndexMesh.h"
#include "CheckML.h"
#include <fstream>
using namespace std;
using namespace glm;

void IndexMesh::render() const
{
	
	if (vVertices.size() > 0) {  // transfer data
	  // transfer the coordinates of the vertices
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, 0, vVertices.data());  // number of coordinates per vertex, type of each coordinate, stride, pointer 
		if (vColors.size() > 0) { // transfer colors
			glEnableClientState(GL_COLOR_ARRAY);
			glColorPointer(4, GL_FLOAT, 0, vColors.data());  // components number (rgba=4), type of each component, stride, pointer  
		}
		if (vTexCoords.size() > 0) {
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glTexCoordPointer(2, GL_DOUBLE, 0, vTexCoords.data());
		}
		if (vNormals.size() > 0)
		{
			glEnableClientState(GL_NORMAL_ARRAY);
			glNormalPointer(GL_DOUBLE, 0, vNormals.data());
		}
		if (vIndices != nullptr)
		{
			glEnableClientState(GL_INDEX_ARRAY);
			glIndexPointer(GL_UNSIGNED_INT, 0, vIndices);
		}

		draw();

		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_INDEX_ARRAY);
	}
}

void IndexMesh::draw() const
{
	glDrawElements(mPrimitive, nNumIndices, GL_UNSIGNED_INT, vIndices);
}

IndexMesh* IndexMesh::generaAnilloCuadradoIndexado()
{
	IndexMesh* mesh = new IndexMesh();

	mesh->mPrimitive = GL_TRIANGLE_STRIP;
	mesh->mNumVertices = 8;
	mesh->nNumIndices = 10;

	mesh->vColors.reserve(mesh->mNumVertices);
	mesh->vIndices = new GLuint[mesh->nNumIndices];
	mesh->vNormals.reserve(mesh->mNumVertices);
	mesh->vVertices.reserve(mesh->mNumVertices);

	mesh->vColors = { {0.0, 0.0, 0.0, 1.0}, {1.0, 0.0, 0.0, 1.0}, {0.0, 1.0, 0.0, 1.0}, {0.0, 0.0, 1.0, 1.0},
		{1.0, 1.0, 0.0, 1.0}, {1.0, 0.0, 1.0, 1.0}, {0.0, 1.0, 1.0, 1.0}, {1.0, 0.0, 0.0, 1.0}};
	mesh->vVertices = { {30.0, 30.0, 0.0 }, { 10.0, 10.0, 0.0 }, { 70.0, 30.0, 0.0 }, { 90.0, 10.0, 0.0 },
		{ 70.0, 70.0, 0.0 }, { 90.0, 90.0, 0.0 }, { 30.0, 70.0, 0.0 }, { 10.0, 90.0, 0.0 } };

	for (int i = 0; i < mesh->mNumVertices; i++)
		mesh->vNormals.emplace_back((0, 0, 1));
	unsigned int vIndices[] =
	{ 0, 1, 2, 3, 4, 5, 6, 7, 0, 1 };
	return mesh;
}

IndexMesh* IndexMesh::generaCuboConTapasIndexado(GLdouble l)
{
	IndexMesh* mesh = new IndexMesh();

	mesh->mPrimitive = GL_TRIANGLE_STRIP;
	mesh->mNumVertices = 8;
	mesh->nNumIndices = 36;

	mesh->vColors.reserve(mesh->mNumVertices);
	mesh->vIndices = new GLuint[mesh->nNumIndices];
	mesh->vNormals.reserve(mesh->mNumVertices);
	mesh->vVertices.reserve(mesh->mNumVertices);

	mesh->vColors = { {0.0, 0.5, 0.0, 1.0}, {0.0, 0.5, 0.0, 1.0}, {0.0, 0.5, 0.0, 1.0}, {0.0, 0.5, 0.0, 1.0},
		{0.0, 0.5, 0.0, 1.0}, {0.0, 0.5, 0.0, 1.0}, {0.0, 0.5, 0.0, 1.0}, {0.0, 0.5, 0.0, 1.0} };
	mesh->vVertices = { {l, l, -l}, {l, -l, -l}, {l, l, l}, {l, -l, l}, {-l, l, l}, {-l, -l, l}, {-l, l, -l}, {-l, -l, -l} };
	//mesh->vIndices = { 0, 1, 2, 2, 1, 3, 2, 3, 4, 4, 3, 5, 4, 5, 6, 6, 5, 7, 6, 7, 0, 0, 7, 1, 4, 6, 2, 2, 6, 0, 1, 7, 3, 3, 7, 5 };
	mesh->BuildNormalVectors();
	return mesh;
}

void IndexMesh::BuildNormalVectors()
{
	for (int i = 0; i < mNumVertices; i++)
		vNormals.emplace_back(0, 0, 0);

	int ind = 0;
	for (int i = 0; i < mNumVertices; i++)
	{
		dvec3 a = vVertices[vIndices[ind]];
		ind++;
		dvec3 b = vVertices[vIndices[ind]];
		ind++;
		dvec3 c = vVertices[vIndices[ind]];
		ind++;
		dvec3 n = normalize(cross(b - a, c - a));
		vNormals[i] += n;
	}
}