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

	glBegin(GL_TRIANGLE_STRIP);

	mesh->mNumVertices = 8;
	mesh->nNumIndices = 10;

	mesh->vColors.reserve(mesh->mNumVertices);
	mesh->vIndices = new GLuint[mesh->nNumIndices];
	mesh->vNormals.reserve(mesh->mNumVertices);
	mesh->vVertices.reserve(mesh->mNumVertices);

	mesh->vColors = { {30.0, 30.0, 0.0.}, {10.0, 10.0, 0.0}, {70.0, 30.0, 0.0}, {90.0, 10.0, 0.0},
		{70.0, 70.0, 0.0}, {90.0, 90.0, 0.0}, {30.0, 70.0, 0.0}, {10.0, 90.0, 0.0} };
	mesh->vVertices = { {30.0, 30.0, 0.0 }, { 10.0, 10.0, 0.0 }, { 70.0, 30.0, 0.0 }, { 90.0, 10.0, 0.0 },
		{ 70.0, 70.0, 0.0 }, { 90.0, 90.0, 0.0 }, { 30.0, 70.0, 0.0 }, { 10.0, 90.0, 0.0 } };

	for(int i = 0; i < m)
	/*mesh->vColors = { {30.0, 30.0, 0.0}, {10.0, 10.0, 0.0}, {70.0, 30.0, 0.0}, {90.0, 10.0, 0.0},
		{70.0, 70.0, 0.0}, {90.0, 90.0, 0.0}, {30.0, 70.0, 0.0}, {10.0, 90.0, 0.0} };*/
	/*static float vertices[8][3] = {
		{30.0, 30.0, 0.0}, {10.0, 10.0, 0.0}, {70.0, 30.0, 0.0}, {90.0, 10.0, 0.0},
		{70.0, 70.0, 0.0}, {90.0, 90.0, 0.0}, {30.0, 70.0, 0.0}, {10.0, 90.0, 0.0}
	};*/

	/*static float colors[8][3] = {
		{0.0, 0.0, 0.0}, {1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0},
		{1.0, 1.0, 0.0}, {1.0, 0.0, 1.0}, {0.0, 1.0, 1.0}, {1.0, 0.0, 0.0}
	};*/

	for (int i = 0; i < 10; i++)
	{
		/*glColor3fv(colors[i % 8]);
		glVertex3fv(vertices[i % 8]);*/
		glArrayElement(i % 8);
	}
	glEnd();

	return mesh;
}