#pragma once

#include <GL/glew.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <GLFW/glfw3.h>


class Window
{
public:
	GLFWwindow * window;
public:
	Window(const char* title, int width, int height);
	~Window();
};

