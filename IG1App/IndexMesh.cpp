#include "IndexMesh.h"
#include "CheckML.h"
#include <fstream>
using namespace std;
using namespace glm;

void IndexMesh::render() const
{
	if (vIndices != nullptr)
	{
		glEnableClientState(GL_INDEX_ARRAY);
		glIndexPointer(GL_UNSIGNED_INT, 0, vIndices);
	}

	glDisableClientState(GL_INDEX_ARRAY);
}

void IndexMesh::draw() const
{
	glDrawElements(mPrimitive, nNumIndices, GL_UNSIGNED_INT, vIndices);
}

IndexMesh* IndexMesh::generaAnilloCuadradoIndexado()
{
	IndexMesh* mesh = new IndexMesh();
	static float vertices[8][3] = {
		{30.0, 30.0, 0.0}, {10.0, 10.0, 0.0}, {70.0, 30.0, 0.0}, {90.0, 10.0, 0.0},
		{70.0, 70.0, 0.0}, {90.0, 90.0, 0.0}, {30.0, 70.0, 0.0}, {10.0, 90.0, 0.0}
	};

	static float colors[8][3] = {
		{0.0, 0.0, 0.0}, {1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0},
		{1.0, 1.0, 0.0}, {1.0, 0.0, 1.0}, {0.0, 1.0, 1.0}, {1.0, 0.0, 0.0}
	};

	glBegin(GL_TRIANGLE_STRIP);
	for (int i = 0; i < 10; i++)
	{
		glColor3fv(colors[i % 8]);
		glVertex3fv(vertices[i % 8]);
	}
	glEnd();

	return mesh;
}