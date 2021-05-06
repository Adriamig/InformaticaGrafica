#pragma once

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
};