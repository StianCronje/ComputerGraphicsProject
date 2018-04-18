#pragma once
#include <GL/glew.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <GLFW/glfw3.h>
#include <SOIL.h>
#include "FreeImage.h"

GLuint loadtextures(const char* filename);