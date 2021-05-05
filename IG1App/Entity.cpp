#include "Entity.h"
#include "Texture.h"
#include "IG1App.h"

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
	mMesh = Mesh::generaEstrellaTexCor(re, np, h);
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
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		mTexture->bind(GL_REPLACE);
		upload(aMat);
		mMesh->render();
		mTexture->bind(GL_REPLACE);
		aMat = modelViewMat * rotate(mModelMat, radians(180.0), dvec3(1, 0, 0));
		upload(aMat);
		mMesh->render();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		mTexture->unbind();
	}
}

void Estrella3D::update() {
	mModelMat = rotate(mModelMat, radians(angZ), dvec3(0, 0, 1));
	mModelMat = rotate(mModelMat, radians(angY), dvec3(0, 1, 0));
}

//-------------------------------------------------------------------------
Caja::Caja(GLdouble ld)
{
	mMesh = Mesh::generaContCuboTexCor(ld);
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
		glEnable(GL_CULL_FACE);
		upload(aMat);
		mTexture->bind(GL_REPLACE);
		glCullFace(GL_BACK);
		mMesh->render();
		mTexture2->bind(GL_REPLACE);
		glCullFace(GL_FRONT);
		mMesh->render();
		mTexture->unbind();
		glDisable(GL_CULL_FACE);
	}
}

void Caja::update() {
	mModelMat = rotate(mModelMat, radians(angZ), dvec3(0, 0, 1));
}

CajaConFondo::CajaConFondo(GLdouble ld)
{
	mMesh = Mesh::generaContCuboConFondo(ld);
}

CajaConFondo::~CajaConFondo()
{
	delete mMesh;
	mMesh = nullptr;
}

void CajaConFondo::render(dmat4 const& modelViewMat) const
{
	if (mMesh != nullptr)
	{
		dmat4 aMat = modelViewMat * mModelMat;
		glEnable(GL_CULL_FACE);
		upload(aMat);
		mTexture->bind(GL_REPLACE);
		glCullFace(GL_BACK);
		mMesh->render();
		mTexture2->bind(GL_REPLACE);
		glCullFace(GL_FRONT);
		mMesh->render();
		mTexture->unbind();
		glDisable(GL_CULL_FACE);
	}
}

void CajaConFondo::update() {
	mModelMat = rotate(mModelMat, radians(angZ), dvec3(0, 0, 1));
}

Suelo::Suelo(GLdouble w, GLdouble h, GLuint rw, GLuint rh)
{
	mMesh = Mesh::generaRectanguloTexCor(w, h, rw, rh);
}

Suelo::~Suelo()
{
	delete mMesh;
	mMesh = nullptr;
}

void Suelo::render(dmat4 const& modelViewMat) const
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

Foto::Foto()
{
	mMesh = Mesh::generaRectanguloText(100, 50);
}

Foto::~Foto()
{
	delete mMesh;
	mMesh = nullptr;
}

void Foto::render(dmat4 const& modelViewMat) const
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

void Foto::update() {
	mTexture->loadColorBuffer(IG1App::s_ig1app.winWidth(), IG1App::s_ig1app.winHeight(), GL_FRONT);
}

Vidriera::Vidriera(GLdouble ld, GLdouble h)
{
	mMesh = Mesh::generaVidriera(ld, h);
}

Vidriera::~Vidriera()
{
	delete mMesh;
	mMesh = nullptr;
}

void Vidriera::render(dmat4 const& modelViewMat) const
{
	if (mMesh != nullptr)
	{
		dmat4 aMat = modelViewMat * mModelMat;
		glDepthMask(GL_FALSE);
		glEnable(GL_BLEND);
		glBlendFunc(1, GL_ONE_MINUS_SRC_ALPHA);
		mTexture->bind(GL_REPLACE);
		upload(aMat);
		mMesh->render();
		mTexture->unbind();
		//glBlendFunc(1, 0);
		glDepthMask(GL_TRUE);
		glDisable(GL_BLEND);
	}
}