#pragma once
#include <vector>
#include <GL\glew.h>
#include <GL\glut.h>
#include <GLFW\glfw3.h>

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