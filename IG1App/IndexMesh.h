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
	virtual ~IndexMesh();
	virtual void render() const;
	virtual void draw() const;
	static IndexMesh* generaAnilloCuadradoIndexado();
	static IndexMesh* generaCuboConTapasIndexado(GLdouble l);
	static IndexMesh* generateGrid(GLdouble lado, GLuint nDiv);
	static IndexMesh* generateGridTex(GLdouble lado, GLuint nDiv);
	void buildNormalVectors();
};

class MbR : public IndexMesh
{
public:
	MbR(int mm, int nn, glm::dvec3* perfill);
	~MbR() { delete perfil; perfil = nullptr; };
	static MbR* generaMallaIndexadaPorRevolucion(int mm, int nn, glm::dvec3* perfil);
protected:
	int m; //número de puntos del perfil
	int n; //número de rotaciones (muestras) que se toman
	glm::dvec3* perfil; //perfil original en el plano XY

};