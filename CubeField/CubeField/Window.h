#pragma once

#include <GL/glew.h>
#include <GL/glut.h>
#include <GLFW/glfw3.h>


class Window
{
public:
	GLFWwindow * window;
public:
	Window(const char* title, int width, int height);
	~Window();
};

