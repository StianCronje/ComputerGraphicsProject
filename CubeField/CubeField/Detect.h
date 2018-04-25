#pragma once
#include <vector>
#include <GL/glew.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include "Model.h"

class Detect
{
private:
	Model* one;
	Model* two;
public:
	Detect(Model* one, Model* two);
public:
	bool compare();
};