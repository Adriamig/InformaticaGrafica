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
		/*glLineWidth(1);*/
		glColor3d(1, 1, 1);

	}
}

PoligonoConFondo::PoligonoConFondo(GLdouble rd, GLuint numL)
{
	mMesh = Mesh::generaPoligonoConFondo(numL, rd);
}

PoligonoConFondo::~PoligonoConFondo()
{
	delete mMesh;
	mMesh = nullptr;
}

void PoligonoConFondo::render(dmat4 const& modelViewMat) const
{
	if (mMesh != nullptr)
	{
		dmat4 aMat = modelViewMat * mModelMat;  // glm matrix multiplication
		upload(aMat);
		glDepthMask(GL_FALSE);
		glEnable(GL_BLEND);
		glBlendFunc(1, GL_ONE_MINUS_SRC_ALPHA);
		mTexture->bind(GL_REPLACE);
		mMesh->render();
		mTexture->unbind();
		glDepthMask(GL_TRUE);
		glDisable(GL_BLEND);
		/*glLineWidth(1);*/

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

AnilloCuadrado::AnilloCuadrado()
{
	mMesh = IndexMesh::generaAnilloCuadradoIndexado();
}

AnilloCuadrado::~AnilloCuadrado()
{
	delete mMesh;
	mMesh = nullptr;
}

void AnilloCuadrado::render(dmat4 const& modelViewMat) const
{
	if (mMesh != nullptr)
	{
		dmat4 aMat = modelViewMat * mModelMat;
		upload(aMat);
		glEnable(GL_COLOR_MATERIAL);
		mMesh->render();
		glDisable(GL_COLOR_MATERIAL);
	}
}

Cubo::Cubo(GLdouble l)
{
	mMesh = IndexMesh::generaCuboConTapasIndexado(l);
}

Cubo::~Cubo()
{
	delete mMesh;
	mMesh = nullptr;
}

void Cubo::render(dmat4 const& modelViewMat) const
{
	if (mMesh != nullptr)
	{
		dmat4 aMat = modelViewMat * mModelMat;
		upload(aMat);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glEnable(GL_COLOR_MATERIAL);
		glColor4dv(value_ptr(mColor));
		mMesh->render();
		glDisable(GL_COLOR_MATERIAL);
		glColor4dv(value_ptr(blanco));
	}
}

Cone::Cone(GLdouble h, GLdouble r, GLuint n)
{
	int m = 3;
	dvec3* perfil = new dvec3[m];
	perfil[0] = dvec3(0.5, 0.0, 0.0);
	perfil[1] = dvec3(r, 0.0, 0.0);
	perfil[2] = dvec3(0.5, h, 0.0);
	this->mMesh = MbR::generaMallaIndexadaPorRevolucion(m, n, perfil); //new MbR(m, n, perfil);
}

void Cone::render(dmat4 const& modelViewMat) const
{
	if (mMesh != nullptr)
	{
		dmat4 aMat = modelViewMat * mModelMat;
		upload(aMat);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glEnable(GL_COLOR_MATERIAL);
		glColor4dv(value_ptr(mColor));
		mMesh->render();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDisable(GL_COLOR_MATERIAL);
		glColor4dv(value_ptr(blanco));
	}
}

Esfera::Esfera(GLdouble r, GLdouble p, GLuint m)
{
	dvec3* perfil = new dvec3[m + 1];
	for (int i = 0; i < m - 1; i++)
	{
		double x = r * cos(radians(-90.0 + (180.0 / m) * i));
		double y = r * sin(radians(-90.0 + (180.0 / m) * i));
		perfil[i] = dvec3(x, y, 0);
	}
	perfil[m - 1] = dvec3(0, r, 0);
	this->mMesh = MbR::generaMallaIndexadaPorRevolucion(m, p, perfil); //new MbR(m, n, perfil);
}

Esfera::~Esfera()
{
	delete mMesh; delete material;
	mMesh = nullptr;
}

void Esfera::render(dmat4 const& modelViewMat) const
{
	if (mMesh != nullptr)
	{
		dmat4 aMat = modelViewMat * mModelMat;
		upload(aMat);
		if (material != nullptr)
			material->upload();
		else {
			glEnable(GL_COLOR_MATERIAL);
			glColor4dv(value_ptr(mColor));
		}
		mMesh->render();
		if (material == nullptr) {
			glDisable(GL_COLOR_MATERIAL);
			glColor4dv(value_ptr(blanco));
		}
	}
	
}

Grid::Grid(GLdouble lado, GLuint nDiv)
{
	mMesh = IndexMesh::generateGridTex(lado, nDiv);
}

Grid::~Grid()
{
	delete mMesh;
	mMesh = nullptr;
}

void Grid::render(dmat4 const& modelViewMat) const
{
	if (mMesh != nullptr)
	{
		dmat4 aMat = modelViewMat * mModelMat;
		glPolygonMode(GL_BACK, GL_LINE);
		mTexture->bindLight(GL_MODULATE);
		upload(aMat);
		mMesh->render();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		mTexture->unbind();
	}
}

CompoundEntity::CompoundEntity()
{

}

CompoundEntity::~CompoundEntity()
{
	for (Abs_Entity* el : gObjects)
	{
		delete el; el = nullptr;
	}
	gObjects.clear();
}

void CompoundEntity::addEntity(Abs_Entity* ae)
{
	gObjects.push_back(ae);
}

void CompoundEntity::render(dmat4 const& modelViewMat) const
{
	dmat4 aMat = modelViewMat * mModelMat;
	upload(aMat);
	for (Abs_Entity* el : gObjects)
		el->render(aMat);
}

TIE::TIE(Texture* noche, SpotLight* f) : CompoundEntity()
{
	foco = f;

	Cylinder* shaft = new Cylinder(25.0, 25.0, 500.0);
	shaft->setModelMat(translate(shaft->modelMat(), dvec3(0, 0, -250)));
	addEntity(shaft);

	CompoundEntity* front = new CompoundEntity();
	addEntity(front);
	Cylinder* internCylinder = new Cylinder(100.0, 100.0, 250.0);
	internCylinder->setModelMat(rotate(internCylinder->modelMat(), radians(90.0), dvec3(0, 1, 0)));
	internCylinder->setModelMat(translate(internCylinder->modelMat(), dvec3(0, 0, -100)));
	front->addEntity(internCylinder);
	Disk* disk = new Disk(0.0, 100.0, 50);
	disk->setModelMat(rotate(disk->modelMat(), radians(90.0), dvec3(0, 1, 0)));
	disk->setModelMat(translate(disk->modelMat(), dvec3(0, 0, 150)));
	front->addEntity(disk);

	Sphere* core = new Sphere(150.0);
	addEntity(core);

	PartialDisk* wingL = new PartialDisk(0, 300, 360, 360);
	wingL->setTexture(noche);
	wingL->setModelMat(rotate(wingL->modelMat(), radians(90.0), dvec3(0, 0, 1)));
	wingL->setModelMat(translate(wingL->modelMat(), dvec3(0, 0, 250)));
	addEntity(wingL);

	PartialDisk* wingR = new PartialDisk(0, 300, 360, 360);
	wingR->setTexture(noche);
	wingR->setModelMat(rotate(wingR->modelMat(), radians(90.0), dvec3(0, 0, 1)));
	wingR->setModelMat(translate(wingR->modelMat(), dvec3(0, 0, -250)));
	addEntity(wingR);
}

void TIE::render(glm::dmat4 const& modelViewMat) const {
	dmat4 aMat = modelViewMat * mModelMat;
	upload(aMat);

	foco->upload(aMat);

	for (Abs_Entity* el : gObjects)
		el->render(aMat);
}

GridCube::GridCube(Texture* stones, Texture* checker) {
	auto* top = new Grid(400, 100);
	top->setTexture(checker);
	top->setModelMat(translate(top->modelMat(), dvec3(0, 200, 0)));
	top->setModelMat(rotate(top->modelMat(), radians(90.0), dvec3(0, 1, 0)));
	addEntity(top);
	auto* xPos = new Grid(400, 100);
	xPos->setTexture(stones);
	xPos->setModelMat(translate(xPos->modelMat(), dvec3(200, 0, 0)));
	xPos->setModelMat(rotate(xPos->modelMat(), radians(-90.0), dvec3(0, 0, 1)));
	addEntity(xPos);
	auto* xNeg = new Grid(400, 100);
	xNeg->setTexture(stones);
	xNeg->setModelMat(translate(xNeg->modelMat(), dvec3(-200, 0, 0)));
	xNeg->setModelMat(rotate(xNeg->modelMat(), radians(180.0), dvec3(1, 0, 0)));
	xNeg->setModelMat(rotate(xNeg->modelMat(), radians(90.0), dvec3(0, 0, 1)));
	addEntity(xNeg);
	auto* zPos = new Grid(400, 100);
	zPos->setTexture(stones);
	zPos->setModelMat(translate(zPos->modelMat(), dvec3(0, 0, 200)));
	zPos->setModelMat(rotate(zPos->modelMat(), radians(-90.0), dvec3(0, 0, 1)));
	zPos->setModelMat(rotate(zPos->modelMat(), radians(90.0), dvec3(1, 0, 0)));
	addEntity(zPos);
	auto* zNeg = new Grid(400, 100);
	zNeg->setTexture(stones);
	zNeg->setModelMat(translate(zNeg->modelMat(), dvec3(0, 0, -200)));
	zNeg->setModelMat(rotate(zNeg->modelMat(), radians(-90.0), dvec3(0, 0, 1)));
	zNeg->setModelMat(rotate(zNeg->modelMat(), radians(-90.0), dvec3(1, 0, 0)));
	addEntity(zNeg);
	auto* bottom = new Grid(400, 100);
	bottom->setTexture(checker);
	bottom->setModelMat(translate(bottom->modelMat(), dvec3(0, -200, 0)));
	bottom->setModelMat(rotate(bottom->modelMat(), radians(180.0), dvec3(0, 0, 1)));
	addEntity(bottom);
}

Flota::Flota(Texture* noche, SpotLight* f1, SpotLight* f2, SpotLight* f3) {
	// Tiene que ser compund entity los 3
	auto* nave = new TIE(noche, f1);
	nave->setModelMat(translate(nave->modelMat(), dvec3(0, 300, 0)));
	nave->setModelMat(scale(nave->modelMat(), dvec3(0.1, 0.1, 0.1)));
	addEntity(nave);

	auto* nave2 = new TIE(noche, f2);
	nave2->setModelMat(rotate(nave2->modelMat(), radians(15.0), dvec3(1, 0, 0)));
	nave2->setModelMat(rotate(nave2->modelMat(), radians(5.0), dvec3(0, 0, 1)));
	nave2->setModelMat(translate(nave2->modelMat(), dvec3(0, 300, 0)));
	nave2->setModelMat(scale(nave2->modelMat(), dvec3(0.1, 0.1, 0.1)));
	addEntity(nave2);

	auto* nave3 = new TIE(noche, f3);
	nave3->setModelMat(rotate(nave3->modelMat(), radians(-15.0), dvec3(1, 0, 0)));
	nave3->setModelMat(rotate(nave3->modelMat(), radians(5.0), dvec3(0, 0, 1)));
	nave3->setModelMat(translate(nave3->modelMat(), dvec3(0, 300, 0)));
	nave3->setModelMat(scale(nave3->modelMat(), dvec3(0.1, 0.1, 0.1)));
	addEntity(nave3);
}