#pragma once
#include <iostream>
#include <vector>
#include <array>
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
	Model * one;
	Model* two;
	std::array<glm::vec3, 2> twoBounds;
public:
	Detect(Model* one, Model* two);
	Detect(Model* one, std::array<glm::vec3, 2> two);
public:
	bool compare(float set);
	bool comapre_bounds();
};