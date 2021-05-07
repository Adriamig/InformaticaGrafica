#pragma once

#include "Entity.h"

class QuadricEntity : public Abs_Entity
{
public:
	QuadricEntity();
	~QuadricEntity() { gluDeleteQuadric(q); };
protected:
	GLUquadricObj* q;
};

class Sphere : public QuadricEntity
{
public:
	Sphere(GLdouble r);
	void render(glm::dmat4 const& modelViewMat) const;
protected:
	GLdouble r;
};

class Cylinder : public QuadricEntity
{
public:
	Cylinder(GLdouble br, GLdouble tr, GLdouble h);
	void render(glm::dmat4 const& modelViewMat) const;
protected:
	GLdouble br, tr, h;
};

class Disk : public QuadricEntity
{
public:
	Disk(GLdouble ir, GLdouble outR, GLuint slices);
	void render(glm::dmat4 const& modelViewMat) const;
protected:
	GLdouble ir, outR;
	GLuint slices;
};

class PartialDisk : public QuadricEntity
{
public:
	PartialDisk(GLdouble ir, GLdouble outR, GLdouble st, GLdouble sw);
	void render(glm::dmat4 const& modelViewMat) const;
protected:
	GLdouble ir, outR, stAngle, swAngle;

};
