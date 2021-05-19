#include "QuadricEntity.h"
#include "CheckML.h"

#include <gtc/matrix_transform.hpp>  
#include <gtc/type_ptr.hpp>

QuadricEntity::QuadricEntity()
{
	q = gluNewQuadric();
}

Sphere::Sphere(GLdouble rr)
{
	r = rr;
}

void Sphere::render(glm::dmat4 const& modelViewMat) const
{
	glm::dmat4 aMat = modelViewMat * mModelMat;
	upload(aMat);

	glEnable(GL_COLOR_MATERIAL);
	//glColor3f(0.0, 0.25, 0.41);
    glColor3f(0.0, 0.0, 1.0);

	gluQuadricDrawStyle(q, GLU_FILL);
	gluSphere(q, r, 50, 50);

	glColor3f(1.0, 1.0, 1.0);
}

Cylinder::Cylinder(GLdouble brr, GLdouble trr, GLdouble hh)
{
	br = brr;
	tr = trr;
	h = hh;
}

void Cylinder::render(glm::dmat4 const& modelViewMat) const
{
	glm::dmat4 aMat = modelViewMat * mModelMat;
	upload(aMat);

	glEnable(GL_COLOR_MATERIAL);
	glColor3f(0.0, 0.25, 0.41);

	gluQuadricDrawStyle(q, GLU_FILL);
	gluCylinder(q, br, tr, h, 50, 50);

	glColor3f(1.0, 1.0, 1.0);
}

Disk::Disk(GLdouble irr, GLdouble outRR, GLuint slicess)
{
	ir = irr;
	outR = outRR;
	slices = slicess;
}

void Disk::render(glm::dmat4 const& modelViewMat) const
{
	glm::dmat4 aMat = modelViewMat * mModelMat;
	upload(aMat);

	glEnable(GL_COLOR_MATERIAL);
	glColor3f(0.0, 0.25, 0.41);

	gluQuadricDrawStyle(q, GLU_FILL);
	gluDisk(q, ir, outR, slices, 50);

	glColor3f(1.0, 1.0, 1.0);
}

PartialDisk::PartialDisk(GLdouble irr, GLdouble outRR, GLdouble st, GLdouble sw)
{
	ir = irr;
	outR = outRR;
	stAngle = st;
	swAngle = sw;
}

void PartialDisk::render(glm::dmat4 const& modelViewMat) const
{
	glm::dmat4 aMat = modelViewMat * mModelMat;
	upload(aMat);
	glDepthMask(GL_FALSE);
	glEnable(GL_BLEND);
	glBlendFunc(1, GL_ONE_MINUS_SRC_ALPHA);
	mTexture->bind(GL_REPLACE);
	//glColor3f(0.0, 0.25, 0.41);

	gluQuadricTexture(q, GL_TRUE);
	gluPartialDisk(q, ir, outR, 6, 50, stAngle, swAngle);

	//glColor3f(1.0, 1.0, 1.0);
	mTexture->unbind();
	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
}