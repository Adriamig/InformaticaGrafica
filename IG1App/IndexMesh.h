#pragma once
#include <GL/freeglut.h>
#include <glm.hpp>
#include <vector>
#include "Mesh.h"

class IndexMesh : public Mesh
{
protected:
	GLuint* vIndices = nullptr;
	GLuint nNumIndices = 0;
public:
	IndexMesh() { mPrimitive = GL_TRIANGLES; }
	~IndexMesh() { delete[] vIndices; }
	virtual void render() const;
	virtual void draw() const;
	static IndexMesh* generaAnilloCuadradoIndexado();
	static IndexMesh* generaCuboConTapasIndexado(GLdouble l);
	void buildNormalVectors();
};

class MbR : public IndexMesh
{
public:
	MbR(GLuint mm, GLuint nn, glm::dvec3* perfill);
	~MbR() { delete perfil; };
	static MbR* generaMallaIndexadaPorRevolucion(GLuint mm, GLuint nn, glm::dvec3* perfil);
protected:
	GLuint m; //número de puntos del perfil
	GLuint n; //número de rotaciones (muestras) que se toman
	glm::dvec3* perfil; //perfil original en el plano XY

};