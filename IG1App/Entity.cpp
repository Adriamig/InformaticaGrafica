#include "Entity.h"
#include "Texture.h"

#include <gtc/matrix_transform.hpp>  
#include <gtc/type_ptr.hpp>

using namespace glm;

//-------------------------------------------------------------------------

void Abs_Entity::upload(dmat4 const& modelViewMat) const
{
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixd(value_ptr(modelViewMat));  // transfers modelView matrix to the GPU
}
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

EjesRGB::EjesRGB(GLdouble l) : Abs_Entity()
{
	mMesh = Mesh::createRGBAxes(l);
}
//-------------------------------------------------------------------------

EjesRGB::~EjesRGB()
{
	delete mMesh; mMesh = nullptr;
};
//-------------------------------------------------------------------------

void EjesRGB::render(dmat4 const& modelViewMat) const
{
	if (mMesh != nullptr) {
		dmat4 aMat = modelViewMat * mModelMat;  // glm matrix multiplication
		upload(aMat);
		glLineWidth(2);
		mMesh->render();
		glLineWidth(1);
	}
}

Poligono::Poligono(GLdouble rd, GLuint numL, dvec4 color)
{
	mMesh = Mesh::generaPoligono(numL, rd);
	setColor(color);
}

Poligono::~Poligono()
{
	delete mMesh;
	mMesh = nullptr;
}

void Poligono::render(dmat4 const& modelViewMat) const
{
	if (mMesh != nullptr)
	{
		dmat4 aMat = modelViewMat * mModelMat;  // glm matrix multiplication
		upload(aMat);
		glColor3d(mColor.r, mColor.g, mColor.b);
		mMesh->render();
		glLineWidth(1);
		glColor3d(1, 1, 1);
	}
}

Sierpinski::Sierpinski(GLdouble rd, GLuint numP, dvec4 color)
{
	mMesh = Mesh::generaSierpinski(numP, rd);
	setColor(color);
}

Sierpinski::~Sierpinski()
{
	delete mMesh;
	mMesh = nullptr;
}

void Sierpinski::render(dmat4 const& modelViewMat) const
{
	if (mMesh != nullptr)
	{
		dmat4 aMat = modelViewMat * mModelMat;
		upload(aMat);
		glColor4dv(value_ptr(mColor));
		glPointSize(2);
		mMesh->render();
		glPointSize(1);
		glColor4dv(value_ptr(blanco));
	}
}

TrianguloRGB::TrianguloRGB(GLdouble rd)
{
	mMesh = Mesh::generaTrianguloRGB(rd);
}

TrianguloRGB::~TrianguloRGB()
{
	delete mMesh;
	mMesh = nullptr;
}

void TrianguloRGB::render(dmat4 const& modelViewMat) const
{
	if (mMesh != nullptr)
	{
		dmat4 aMat = modelViewMat * mModelMat;
		upload(aMat);
		glPolygonMode(GL_BACK, GL_POINT);
		mMesh->render();
		glColor4dv(value_ptr(blanco));
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

void TrianguloRGB::update() {
	ang1++; ang2--;
	double x = 200 * sin(radians(ang1));
	double y = 200 * cos(radians(ang1));
	mModelMat = translate(dmat4(1), dvec3(x, y, 0));
	mModelMat = rotate(mModelMat, radians(ang2), dvec3(0, 0, 1));
}

RectanguloRGB::RectanguloRGB(GLdouble w, GLdouble h)
{
	mMesh = Mesh::generaRectanguloRGB(w, h);
}

RectanguloRGB::~RectanguloRGB()
{
	delete mMesh;
	mMesh = nullptr;
}

void RectanguloRGB::render(dmat4 const& modelViewMat) const
{
	if (mMesh != nullptr)
	{
		dmat4 aMat = modelViewMat * mModelMat;
		upload(aMat);
		glPolygonMode(GL_BACK, GL_LINE);
		mMesh->render();
		glColor4dv(value_ptr(blanco));
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}
//-------------------------------------------------------------------------

Estrella3D::Estrella3D(GLdouble re, GLuint np, GLdouble h)
{
	mMesh = Mesh::generaEstrella3D(re, np, h);
}

Estrella3D::~Estrella3D()
{
	delete mMesh;
	mMesh = nullptr;
}

void Estrella3D::render(dmat4 const& modelViewMat) const
{
	if (mMesh != nullptr)
	{
		dmat4 aMat = modelViewMat * mModelMat;
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		upload(aMat);
		mMesh->render();
		aMat = modelViewMat * rotate(mModelMat, radians(180.0), dvec3(1, 0, 0));
		upload(aMat);
		mMesh->render();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

void Estrella3D::update() {
	mModelMat = rotate(mModelMat, radians(angZ), dvec3(0, 0, 1));
	mModelMat = rotate(mModelMat, radians(angY), dvec3(0, 1, 0));
}
//-------------------------------------------------------------------------
Caja::Caja(GLdouble ld)
{
	mMesh = Mesh::generaContCubo(ld);
}

Caja::~Caja()
{
	delete mMesh;
	mMesh = nullptr;
}

void Caja::render(dmat4 const& modelViewMat) const
{
	if (mMesh != nullptr)
	{
		dmat4 aMat = modelViewMat * mModelMat;
		mTexture->bind(GL_REPLACE);
		upload(aMat);
		mMesh->render();
		mTexture->unbind();
	}
}