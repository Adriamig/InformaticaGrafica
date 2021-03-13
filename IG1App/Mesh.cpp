#include "Mesh.h"
#include "CheckML.h"
#include <fstream>
using namespace std;
using namespace glm;

//-------------------------------------------------------------------------

void Mesh::draw() const
{
	glDrawArrays(mPrimitive, 0, size());   // primitive graphic, first index and number of elements to be rendered
}
//-------------------------------------------------------------------------

void Mesh::render() const
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

		draw();

		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	}
}
//-------------------------------------------------------------------------

Mesh* Mesh::createRGBAxes(GLdouble l)
{
	Mesh* mesh = new Mesh();

	mesh->mPrimitive = GL_LINES;

	mesh->mNumVertices = 6;
	mesh->vVertices.reserve(mesh->mNumVertices);

	// X axis vertices
	mesh->vVertices.emplace_back(0.0, 0.0, 0.0);
	mesh->vVertices.emplace_back(l, 0.0, 0.0);
	// Y axis vertices
	mesh->vVertices.emplace_back(0, 0.0, 0.0);
	mesh->vVertices.emplace_back(0.0, l, 0.0);
	// Z axis vertices
	mesh->vVertices.emplace_back(0.0, 0.0, 0.0);
	mesh->vVertices.emplace_back(0.0, 0.0, l);

	mesh->vColors.reserve(mesh->mNumVertices);
	// X axis color: red  (Alpha = 1 : fully opaque)
	mesh->vColors.emplace_back(1.0, 0.0, 0.0, 1.0);
	mesh->vColors.emplace_back(1.0, 0.0, 0.0, 1.0);
	// Y axis color: green
	mesh->vColors.emplace_back(0.0, 1.0, 0.0, 1.0);
	mesh->vColors.emplace_back(0.0, 1.0, 0.0, 1.0);
	// Z axis color: blue
	mesh->vColors.emplace_back(0.0, 0.0, 1.0, 1.0);
	mesh->vColors.emplace_back(0.0, 0.0, 1.0, 1.0);

	return mesh;
}

Mesh* Mesh::generaPoligono(GLuint numL, GLdouble rd)
{
	Mesh* mesh = new Mesh();

	mesh->mPrimitive = GL_LINE_LOOP;

	mesh->mNumVertices = numL;
	mesh->vVertices.reserve(mesh->mNumVertices);

	for (int i = 0; i < numL; i++)
	{
		double x = rd * cos(radians(90.0 + (360.0 / numL) * i));
		double y = rd * sin(radians(90.0 + (360.0 / numL) * i));
		mesh->vVertices.emplace_back(x, y, 0.0);
	}
	return mesh;
}

Mesh* Mesh::generaSierpinski(GLuint numP, GLdouble rd)
{
	Mesh* triangulo = generaPoligono(3, rd);
	Mesh* mesh = new Mesh();

	mesh->mPrimitive = GL_POINTS;
	mesh->mNumVertices = numP;
	mesh->vVertices.reserve(mesh->mNumVertices);

	dvec3 rVertice = triangulo->vVertices[rand() % 3];
	dvec3 ultVertice = rVertice;
	mesh->vVertices.emplace_back(rVertice.x, rVertice.y, rVertice.z);

	for (int i = 0; i < numP; i++) {
		rVertice = triangulo->vVertices[rand() % 3];
		ultVertice = { (ultVertice.x + rVertice.x) / 2.0, (ultVertice.y + rVertice.y) / 2.0 , 0 };
		mesh->vVertices.emplace_back(ultVertice);
	}

	delete triangulo; triangulo = nullptr;
	return mesh;
}

Mesh* Mesh::generaTrianguloRGB(GLdouble rd)
{
	Mesh* mesh = generaPoligono(3, rd);
	mesh->mPrimitive = GL_TRIANGLES;
	mesh->vColors.reserve(mesh->mNumVertices);
	mesh->vColors.emplace_back(1, 0, 0, 1);
	mesh->vColors.emplace_back(0, 1, 0, 1);
	mesh->vColors.emplace_back(0, 0, 1, 1);
	return mesh;
}

Mesh* Mesh::generaRectangulo(GLdouble w, GLdouble h)
{
	Mesh* mesh = new Mesh();

	mesh->mPrimitive = GL_TRIANGLE_STRIP;
	mesh->mNumVertices = 4;
	mesh->vVertices.reserve(mesh->mNumVertices);
	mesh->vVertices.emplace_back(-w / 2, h / 2, 0);
	mesh->vVertices.emplace_back(-w / 2, -h / 2, 0);
	mesh->vVertices.emplace_back(w / 2, h / 2, 0);
	mesh->vVertices.emplace_back(w / 2, -h / 2, 0);

	return mesh;
}

Mesh* Mesh::generaRectanguloRGB(GLdouble w, GLdouble h)
{
	Mesh* mesh = generaRectangulo(w, h);
	mesh->vColors.reserve(mesh->mNumVertices);
	mesh->vColors.emplace_back(0.5, 0.5, 1, 1);
	mesh->vColors.emplace_back(1, 0.5, 0.5, 1);
	mesh->vColors.emplace_back(0.5, 1, 0.5, 1);
	mesh->vColors.emplace_back(0.5, 0.5, 1, 1);
	return mesh;
}

//-------------------------------------------------------------------------
Mesh* Mesh::generaEstrella3D(GLdouble re, GLuint np, GLdouble h)
{
	Mesh* mesh = new Mesh();
	GLdouble ri = re / 2;
	GLdouble angi = 90.0 - (90.0 / (np / 2));

	mesh->mPrimitive = GL_TRIANGLE_FAN;
	mesh->mNumVertices = 2 * np + 2;
	mesh->vVertices.reserve(mesh->mNumVertices);
	mesh->vVertices.emplace_back(0, 0, 0);
	for (int i = 0; i <= np; i++) {
		double xe = re * cos(radians(90.0 - (360.0 / np) * i));
		double ye = re * sin(radians(90.0 - (360.0 / np) * i));
		double xi = ri * cos(radians(angi - (360.0 / np) * i));
		double yi = ri * sin(radians(angi - (360.0 / np) * i));
		mesh->vVertices.emplace_back(xe, ye, h);
		mesh->vVertices.emplace_back(xi, yi, h);
	}
	return mesh;
}

//-------------------------------------------------------------------------


Mesh* Mesh::generaContCubo(GLdouble ld) {
	Mesh* mesh = new Mesh();

	GLdouble m = ld / 2;

	mesh->mPrimitive = GL_TRIANGLE_STRIP;
	mesh->mNumVertices = 10;

	mesh->vVertices.reserve(mesh->mNumVertices);
	mesh->vVertices.emplace_back(-m, m, m);
	mesh->vVertices.emplace_back(-m, -m, m);
	mesh->vVertices.emplace_back(m, m, m);
	mesh->vVertices.emplace_back(m, -m, m);
	mesh->vVertices.emplace_back(m, m, -m);
	mesh->vVertices.emplace_back(m, -m, -m);
	mesh->vVertices.emplace_back(-m, m, -m);
	mesh->vVertices.emplace_back(-m, -m, -m);
	mesh->vVertices.emplace_back(-m, m, m);
	mesh->vVertices.emplace_back(-m, -m, m);


	mesh->vTexCoords.reserve(12);
	mesh->vTexCoords.emplace_back(0, 1);
	mesh->vTexCoords.emplace_back(0, 0);
	mesh->vTexCoords.emplace_back(1, 1);
	mesh->vTexCoords.emplace_back(1, 0);
	mesh->vTexCoords.emplace_back(0, 1);
	mesh->vTexCoords.emplace_back(0, 0);
	mesh->vTexCoords.emplace_back(1, 1);
	mesh->vTexCoords.emplace_back(1, 0);
	mesh->vTexCoords.emplace_back(0, 1);
	mesh->vTexCoords.emplace_back(0, 0);
	mesh->vTexCoords.emplace_back(1, 1);
	mesh->vTexCoords.emplace_back(1, 0);

	return mesh;
}