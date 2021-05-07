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
		glVertexPointer(3, GL_DOUBLE, 0, vVertices.data());  // number of coordinates per vertex, type of each coordinate, stride, pointer 
		if (vColors.size() > 0) { // transfer colors
			glEnableClientState(GL_COLOR_ARRAY);
			glColorPointer(4, GL_DOUBLE, 0, vColors.data());  // components number (rgba=4), type of each component, stride, pointer  
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

	mesh->vNormals.reserve(mesh->mNumVertices);
	mesh->vColors.reserve(mesh->mNumVertices);
	mesh->vIndices = new GLuint[]{ 0, 1, 2, 3, 4, 5, 6, 7, 0, 1 };
	mesh->vVertices.reserve(mesh->mNumVertices);

	mesh->vColors.emplace_back(0.0, 0.0, 0.0, 1.0);
	mesh->vColors.emplace_back(1.0, 0.0, 0.0, 1.0);
	mesh->vColors.emplace_back(0.0, 1.0, 0.0, 1.0);
	mesh->vColors.emplace_back(0.0, 0.0, 1.0, 1.0);
	mesh->vColors.emplace_back(1.0, 1.0, 0.0, 1.0);
	mesh->vColors.emplace_back(1.0, 0.0, 1.0, 1.0);
	mesh->vColors.emplace_back(0.0, 1.0, 1.0, 1.0);
	mesh->vColors.emplace_back(1.0, 0.0, 0.0, 1.0);

	mesh->vVertices.emplace_back(30.0, 30.0, 0.0);
	mesh->vVertices.emplace_back(10.0, 10.0, 0.0);
	mesh->vVertices.emplace_back(70.0, 30.0, 0.0);
	mesh->vVertices.emplace_back(90.0, 10.0, 0.0);
	mesh->vVertices.emplace_back(70.0, 70.0, 0.0);
	mesh->vVertices.emplace_back(90.0, 90.0, 0.0);
	mesh->vVertices.emplace_back(30.0, 70.0, 0.0);
	mesh->vVertices.emplace_back(10.0, 90.0, 0.0);

	for (int i = 0; i < mesh->mNumVertices; i++)
		mesh->vNormals.emplace_back((0, 0, 1));
	/*for (int i = 0; i < mesh->nNumIndices; i++)
		mesh->vIndices[i] = (i % 8);*/

	return mesh;
}

IndexMesh* IndexMesh::generaCuboConTapasIndexado(GLdouble l)
{
	IndexMesh* mesh = new IndexMesh();

	mesh->mPrimitive = GL_TRIANGLES;

	mesh->mNumVertices = 8;
	mesh->nNumIndices = 36;

	mesh->vColors.reserve(mesh->mNumVertices);
	mesh->vNormals.reserve(mesh->mNumVertices);
	mesh->vIndices = new GLuint[]{ 0, 1, 2, 2, 1, 3, 2, 3, 4, 4, 3, 5, 4, 5, 6, 6, 5, 7, 6, 7, 0, 0, 7, 1, 4, 6, 2, 2, 6, 0, 1, 7, 3, 3, 7, 5 };
	mesh->vVertices.reserve(mesh->mNumVertices);

	mesh->vVertices.emplace_back(l, l, l);
	mesh->vVertices.emplace_back(l, -l, l);
	mesh->vVertices.emplace_back(l, l, -l);
	mesh->vVertices.emplace_back(l, -l, -l);
	mesh->vVertices.emplace_back(-l, l, -l);
	mesh->vVertices.emplace_back(-l, -l, -l);
	mesh->vVertices.emplace_back(-l, l, l);
	mesh->vVertices.emplace_back(-l, -l, l);
	
	mesh->BuildNormalVectors();
	return mesh;
}

void IndexMesh::BuildNormalVectors()
{
	for (int i = 0; i < mNumVertices; i++)
		vNormals.emplace_back(0, 0, 0);

	for (int ind = 0; ind < nNumIndices; ind+=3)
	{
		dvec3 a = vVertices[vIndices[ind]];

		dvec3 b = vVertices[vIndices[ind+1]];

		dvec3 c = vVertices[vIndices[ind+2]];

		dvec3 n = (cross(b - a, c - a));

		vNormals[vIndices[ind]] += n;

		vNormals[vIndices[ind+1]] += n;

		vNormals[vIndices[ind+2]] += n;

	}

	for (dvec3 v : vNormals)
		v = normalize(v);
}