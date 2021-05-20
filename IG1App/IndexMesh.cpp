#include "IndexMesh.h"
#include "CheckML.h"
#include <fstream>
using namespace std;
using namespace glm;

IndexMesh::~IndexMesh() {
	delete[] vIndices;
}

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

	mesh->buildNormalVectors();
	return mesh;
}

IndexMesh* IndexMesh::generateGrid(GLdouble lado, GLuint nDiv)
{
	IndexMesh* m = new IndexMesh();
	GLdouble incr = lado / nDiv; // incremento para la coordenada x, y la c. z
	GLuint numFC = nDiv + 1; // número de vértices por filas y columnas

	m->mNumVertices = numFC * numFC;
	m->vVertices.reserve(m->mNumVertices);
	GLdouble z = -lado / 2;
	GLdouble x = -lado / 2;
	for (int i = 0; i < numFC; i++)
	{
		for (int j = 0; j < numFC; j++)
			m->vVertices.emplace_back(dvec3(x + j * incr, 0, z + i * incr));
		//m->vVertices[i * numFC + j] = dvec3(x + j * incr, 0, z + i * incr);
	}

	m->nNumIndices = nDiv * nDiv * 6;
	m->vIndices = new GLuint[m->nNumIndices];
	int k = 0;
	for (int i = 0; i < nDiv; i++)
	{
		for (int j = 0; j < nDiv; j++)
		{
			int iv = i * numFC + j;
			m->vIndices[k] = iv;
			k++;
			m->vIndices[k] = iv + numFC;
			k++;
			m->vIndices[k] = iv + 1;
			k++;
			m->vIndices[k] = iv + 1;
			k++;
			m->vIndices[k] = iv + numFC;
			k++;
			m->vIndices[k] = iv + numFC + 1;
			k++;
		}
	}
	m->buildNormalVectors();
	return m;
}

IndexMesh* IndexMesh::generateGridTex(GLdouble lado, GLuint nDiv)
{
	IndexMesh* m = generateGrid(lado, nDiv);
	GLuint numFC = nDiv + 1;

	m->vTexCoords.reserve(m->mNumVertices);
	int s = 0;
	int t = 1;
	GLdouble tC = 1.0 / nDiv;
	for (int i = 0; i < numFC; i++)
	{
		for (int j = 0; j < numFC; j++)
		{
			m->vTexCoords.emplace_back(s + tC * j, t - tC * i);
		}
	}
	return m;
}

void IndexMesh::buildNormalVectors()
{
	for (int i = 0; i < mNumVertices; i++)
		vNormals.emplace_back(0, 0, 0);

	for (int ind = 0; ind < nNumIndices; ind += 3)
	{
		dvec3 a = vVertices[vIndices[ind]];

		dvec3 b = vVertices[vIndices[ind + 1]];

		dvec3 c = vVertices[vIndices[ind + 2]];

		dvec3 n = (cross(b - a, c - a));

		vNormals[vIndices[ind]] += n;

		vNormals[vIndices[ind + 1]] += n;

		vNormals[vIndices[ind + 2]] += n;

	}

	for (int i = 0; i < vNormals.size(); i++) {
		vNormals[i] = normalize(vNormals[i]);
	}
}

MbR::MbR(GLuint mm, GLuint nn, dvec3* perfill)
{
	n = nn;
	m = mm;
	perfil = perfill;
}

MbR::~MbR() {
	delete[] perfil; 
}

MbR* MbR::generaMallaIndexadaPorRevolucion(GLuint mm, GLuint nn, dvec3* perfil)
{
	MbR* mesh = new MbR(mm, nn, perfil);

	mesh->mPrimitive = GL_TRIANGLES;

	mesh->mNumVertices = nn * mm;
	mesh->vVertices.reserve(mesh->mNumVertices);
	mesh->vIndices = new GLuint[nn * (mm - 1) * 6];

	int indice = 0;
	for (int i = 0; i < nn; i++)
	{
		GLdouble theta = i * 360.0 / nn;
		GLdouble c = cos(radians(theta));
		GLdouble s = sin(radians(theta));
		for (int j = 0; j < mm; j++)
		{
			indice = i * mm + j;
			GLdouble x = c * perfil[j].x + s * perfil[j].z;
			GLdouble z = -s * perfil[j].x + c * perfil[j].z;
			mesh->vVertices.emplace_back(x, perfil[j].y, z);
		}
	}

	indice = 0;
	int indiceMayor = 0;
	for (int i = 0; i < nn; i++)
	{
		for (int j = 0; j < mm - 1; j++)
		{
			indice = i * mm + j;
			mesh->vIndices[indiceMayor] = indice;
			indiceMayor++;
			mesh->vIndices[indiceMayor] = (indice + mm) % (nn * mm);
			indiceMayor++;
			mesh->vIndices[indiceMayor] = (indice + mm + 1) % (nn * mm);
			indiceMayor++;
			mesh->vIndices[indiceMayor] = indice;
			indiceMayor++;
			mesh->vIndices[indiceMayor] = (indice + mm + 1) % (nn * mm);
			indiceMayor++;
			mesh->vIndices[indiceMayor] = indice + 1;
			indiceMayor++;
		}
	}

	mesh->buildNormalVectors();
	return mesh;
}