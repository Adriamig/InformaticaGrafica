//#pragma once
#ifndef _H_Scene_H_
#define _H_Scene_H_

#include <GL/freeglut.h>
#include <glm.hpp>

#include "Camera.h"
#include "Entity.h"
#include "Texture.h"
#include "QuadricEntity.h"
#include "Light.h"

#include <vector>

//-------------------------------------------------------------------------

class Scene
{
public:
	static DirLight* dirLight;
	static PosLight* posLight;
	static SpotLight* spotLight;

	Scene() {};
	~Scene();

	Scene(const Scene& s) = delete;  // no copy constructor
	Scene& operator=(const Scene& s) = delete;  // no copy assignment

	void init();
	void update();

	void setLightModel(GLfloat* amb) { glLightModelfv(GL_LIGHT_MODEL_AMBIENT, amb); };

	void render(Camera const& cam) const;

	void changeScene(int id);

	void setTIEsLights(bool active);
	void setLights();

	void orbita();
	void rota();

	//void sceneDirLight(Camera const& cam) const;

protected:
	void free();
	void setGL();
	void resetGL();

	int mId = 0;
	static bool initGL;

	static SpotLight* naveFoco1;
	static SpotLight* naveFoco2;
	static SpotLight* naveFoco3;

	std::vector<Abs_Entity*> gObjects;  // Entities (graphic objects) of the scene
	std::vector<Texture*> gTextures;
	std::vector<Light*> gLights;
};
//-------------------------------------------------------------------------

#endif //_H_Scene_H_

