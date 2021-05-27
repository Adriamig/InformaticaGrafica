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
	DirLight* dirLight;
	PosLight* posLight;
	SpotLight* spotLight;

	Scene(): dirLight(nullptr), posLight(nullptr), spotLight(nullptr) {};
	~Scene() { free(); resetGL(); };

	Scene(const Scene& s) = delete;  // no copy constructor
	Scene& operator=(const Scene& s) = delete;  // no copy assignment

	void init();
	void update();

	void setLightModel(GLfloat* amb) { glLightModelfv(GL_LIGHT_MODEL_AMBIENT, amb); };

	void render(Camera const& cam) const;

	void changeScene(int id);

	//void sceneDirLight(Camera const& cam) const;

protected:
	void free();
	void setGL();
	void setLights();
	void resetGL();

	int mId = 0;
	static bool initGL;

	std::vector<Abs_Entity*> gObjects;  // Entities (graphic objects) of the scene
	std::vector<Texture*> gTextures;
};
//-------------------------------------------------------------------------

#endif //_H_Scene_H_

