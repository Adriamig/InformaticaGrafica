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
	void BuildNormalVectors();
};