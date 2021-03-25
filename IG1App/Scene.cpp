#include "Scene.h"
#include "CheckML.h"
#include <gtc/matrix_transform.hpp>  
#include <gtc/type_ptr.hpp>

using namespace glm;
//-------------------------------------------------------------------------

void Scene::init()
{
	setGL();  // OpenGL settings
	//glEnable(GL_DEPTH_TEST);
	// allocate memory and load resources
	// Lights
	// Textures
	Texture* t = new Texture();
	t->load("..\\Bmps\\container.bmp");
	gTextures.push_back(t);
	Texture* bC = new Texture();
	bC->load("..\\Bmps\\baldosaC.bmp");
	gTextures.push_back(bC);
	Texture* bP = new Texture();
	bP->load("..\\Bmps\\baldosaP.bmp");
	gTextures.push_back(bP);
	Texture* pC = new Texture();
	pC->load("..\\Bmps\\papelC.bmp");
	gTextures.push_back(pC);
	Texture* wV = new Texture();
	wV->load("..\\Bmps\\windowV.bmp", 100);
	gTextures.push_back(wV);

	// Graphics objects (entities) of the scene
	gObjects.push_back(new EjesRGB(400.0));

	if (mId == 0) {
		gObjects.push_back(new Poligono(200, 45, { 1.0, 0, 1.0, 1.0 }));
		gObjects.push_back(new Poligono(200, 3, { 1.0, 1.0, 0, 1.0 }));
		gObjects.push_back(new Sierpinski(200, 2000, { 1.0, 1.0, 0, 1.0 }));
		auto tri = new TrianguloRGB(25);
		// tri->setModelMat(translate(tri->modelMat(), dvec3(100, 100, 0))); 
		// tri->setModelMat(rotate(tri->modelMat(), radians(25.0), dvec3(0, 0, 1)));
		// tri->setModelMat(rotate(tri->modelMat(), radians(-25.0), dvec3(0, 0, 1))); // Prueba de cambio
		// tri->setModelMat(translate(tri->modelMat(), dvec3(200, 0, 0))); // Prueba de cambio
		gObjects.push_back(tri);
		auto rect = new RectanguloRGB(700, 400);
		rect->setModelMat(translate(rect->modelMat(), dvec3(0, 0, -100)));
		gObjects.push_back(rect);
	}
	else if (mId == 1) {
		auto e3D = new Estrella3D(50, 4, 40);
		e3D->setTexture(bP);
		e3D->setModelMat(translate(e3D->modelMat(), dvec3(-150, 150, -150)));
		gObjects.push_back(e3D);
		auto c = new CajaConFondo(50);
		c->setTexture(t, pC);
		c->setModelMat(translate(c->modelMat(), dvec3(-215, 25, -215)));
		gObjects.push_back(c);
		auto s = new Suelo(500, 500, 10, 10);
		s->setTexture(bC);
		s->setModelMat(rotate(s->modelMat(), radians(90.0), dvec3(1, 0, 0)));
		gObjects.push_back(s);
		auto foto = new Foto();
		foto->setModelMat(translate(foto->modelMat(), dvec3(0, 1, 0)));
		foto->setModelMat(rotate(foto->modelMat(), radians(90.0), dvec3(1, 0, 0)));
		foto->setTexture(new Texture());
		gObjects.push_back(foto);
		auto v = new Vidriera(500, 200);
		v->setTexture(wV);
		gObjects.push_back(v);
		//gObjects.push_back(new Caja(100));
	}


}
//-------------------------------------------------------------------------
void Scene::changeScene(int id) {
	mId = id;
	free();
	resetGL();
	init();
}

//-------------------------------------------------------------------------
void Scene::free()
{ // release memory and resources   

	for (Abs_Entity* el : gObjects)
	{
		delete el;  el = nullptr;
	}
	gObjects.clear();

	for (Texture* el : gTextures) {
		delete el;  el = nullptr;
	}
	gTextures.clear();
}
//-------------------------------------------------------------------------
void Scene::setGL()
{
	// OpenGL basic setting
	glClearColor(0.0, 0.0, 0.0, 1.0);  // background color (alpha=1 -> opaque)
	glEnable(GL_DEPTH_TEST);  // enable Depth test 
	glEnable(GL_TEXTURE_2D);

}
//-------------------------------------------------------------------------
void Scene::resetGL()
{
	glClearColor(.0, .0, .0, .0);  // background color (alpha=1 -> opaque)
	glDisable(GL_DEPTH_TEST);  // disable Depth test 
	glDisable(GL_TEXTURE_2D);
}
//-------------------------------------------------------------------------

void Scene::render(Camera const& cam) const
{
	cam.upload();

	for (Abs_Entity* el : gObjects)
	{
		el->render(cam.viewMat());
	}
}
//-------------------------------------------------------------------------

void Scene::update() {
	for (Abs_Entity* el : gObjects)
	{
		el->update();
	}
}