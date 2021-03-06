//#pragma once
#ifndef _H_Entities_H_
#define _H_Entities_H_

#include <GL/freeglut.h>
#include <glm.hpp>

#include "Mesh.h"
#include "Texture.h"
#include "IndexMesh.h"
#include "Material.h"
#include "Light.h"

//-------------------------------------------------------------------------

class Abs_Entity  // abstract class
{
public:
	Abs_Entity() : mModelMat(1.0) {};  // 4x4 identity matrix
	virtual ~Abs_Entity() {};

	Abs_Entity(const Abs_Entity& e) = delete;  // no copy constructor
	Abs_Entity& operator=(const Abs_Entity& e) = delete;  // no copy assignment

	virtual void render(glm::dmat4 const& modelViewMat) const = 0;  // abstract method
	virtual void update() {};

	// modeling matrix
	glm::dmat4 const& modelMat() const { return mModelMat; };
	void setModelMat(glm::dmat4 const& aMat) { mModelMat = aMat; };
	void setColor(glm::dvec4 const& color) { mColor = color; };
	void setTexture(Texture* tex, Texture* text2 = nullptr) { mTexture = tex; mTexture2 = text2; };

protected:

	Mesh* mMesh = nullptr;   // the mesh
	glm::dmat4 mModelMat;    // modeling matrix
	glm::dvec4 mColor;
	glm::dvec4 blanco = { 1.0, 1.0, 1.0, 1.0 };
	Texture* mTexture = nullptr;
	Texture* mTexture2 = nullptr;

	// transfers modelViewMat to the GPU
	virtual void upload(glm::dmat4 const& mModelViewMat) const;
};
//-------------------------------------------------------------------------

class EjesRGB : public Abs_Entity
{
public:
	explicit EjesRGB(GLdouble l);
	~EjesRGB();
	virtual void render(glm::dmat4 const& modelViewMat) const;
};

//-------------------------------------------------------------------------

class Poligono : public Abs_Entity
{
public:
	explicit Poligono(GLdouble rd, GLuint numL, glm::dvec4 color);
	~Poligono();
	virtual void render(glm::dmat4 const& modelViewMat) const;
};

class PoligonoConFondo : public Abs_Entity
{
public:
	explicit PoligonoConFondo(GLdouble rd, GLuint numL);
	~PoligonoConFondo();
	virtual void render(glm::dmat4 const& modelViewMat) const;
};

class Sierpinski : public Abs_Entity
{
public:
	explicit Sierpinski(GLdouble rd, GLuint numP, glm::dvec4 color);
	~Sierpinski();
	virtual void render(glm::dmat4 const& modelViewMat) const;
};

class TrianguloRGB : public Abs_Entity
{
public:
	explicit TrianguloRGB(GLdouble rd);
	~TrianguloRGB();
	virtual void render(glm::dmat4 const& modelViewMat) const;
	virtual void update();
	void setAnimation(bool anim) { animation = anim; };

protected:
	bool animation = false;
	GLdouble ang1 = 0;
	GLdouble ang2 = 0;
};

class RectanguloRGB : public Abs_Entity
{
public:
	explicit RectanguloRGB(GLdouble w, GLdouble h);
	~RectanguloRGB();
	virtual void render(glm::dmat4 const& modelViewMat) const;

};

class Estrella3D : public Abs_Entity
{
public:
	explicit Estrella3D(GLdouble re, GLuint np, GLdouble h);
	~Estrella3D();
	virtual void render(glm::dmat4 const& modelViewMat) const;
	virtual void update();
protected:
	GLdouble angZ = 0.1;
	GLdouble angY = 0.2;
};


class Caja : public Abs_Entity
{
public:
	explicit Caja(GLdouble ld);
	~Caja();
	virtual void render(glm::dmat4 const& modelViewMat) const;
	virtual void update();
protected:
	GLdouble angZ = 0.2;

};

class CajaConFondo : public Abs_Entity
{
public:
	explicit CajaConFondo(GLdouble ld);
	~CajaConFondo();
	virtual void render(glm::dmat4 const& modelViewMat) const;
	virtual void update();
protected:
	GLdouble angZ = 0.2;
};

class Suelo : public Abs_Entity
{
public:
	explicit Suelo(GLdouble w, GLdouble h, GLuint rw, GLuint rh);
	~Suelo();
	virtual void render(glm::dmat4 const& modelViewMat) const;
};

class Foto : public Abs_Entity
{
public:
	explicit Foto();
	~Foto();
	virtual void render(glm::dmat4 const& modelViewMat) const;
	virtual void update();
};

class Vidriera : public Abs_Entity
{
public:
	explicit Vidriera(GLdouble ld, GLdouble h);
	~Vidriera();
	virtual void render(glm::dmat4 const& modelViewMat) const;
};

class AnilloCuadrado : public Abs_Entity
{
public:
	explicit AnilloCuadrado();
	~AnilloCuadrado();
	virtual void render(glm::dmat4 const& modelViewMat) const;
};

class Cubo : public Abs_Entity
{
public:
	explicit Cubo(GLdouble l);
	~Cubo();
	virtual void render(glm::dmat4 const& modelViewMat) const;
};

class Cone : public Abs_Entity
{
public:
	Cone(GLdouble h, GLdouble r, GLuint n);
	virtual void render(glm::dmat4 const& modelViewMat) const;
};

class Grid : public Abs_Entity
{
public:
	Grid(GLdouble lado, GLuint nDiv);
	~Grid();
	virtual void render(glm::dmat4 const& modelViewMat) const;
};


class CompoundEntity : public Abs_Entity
{
public:
	CompoundEntity();
	virtual ~CompoundEntity();
	void addEntity(Abs_Entity* ae);
	virtual void render(glm::dmat4 const& modelViewMat) const;
protected:
	std::vector<Abs_Entity*> gObjects;
};

class TIE : public CompoundEntity
{
public:
	SpotLight* foco;

	TIE(Texture* noche, SpotLight* foco);
	virtual void render(glm::dmat4 const& modelViewMat) const;
};

class GridCube : public CompoundEntity {
public:
	GridCube(Texture* stones, Texture* checker);
};

class Flota : public CompoundEntity {
public:
	Flota(Texture* noche, SpotLight* f1, SpotLight* f2, SpotLight* f3);
	virtual void update();
protected:
	GLdouble ang = 0.2;
	GLdouble ang2 = 0.5;
};

class EntityWithMaterial : public Abs_Entity {
public:
	EntityWithMaterial() : Abs_Entity() { };
	virtual ~EntityWithMaterial() { };
	void setMaterial(Material* matl) { material = matl; };
protected:
	Material* material = nullptr;
};

class Esfera : public EntityWithMaterial
{
public:
	Esfera(GLdouble r, GLdouble p, GLuint m);
	~Esfera();
	virtual void render(glm::dmat4 const& modelViewMat) const;
};

#endif //_H_Entities_H_