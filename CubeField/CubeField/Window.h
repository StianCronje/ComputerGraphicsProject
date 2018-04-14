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
private:
	const char* _title;
	int _width, _height;
	GLFWwindow* _window;
public:
	Window(const char* title, int width, int height);
	~Window();
	void clear() const;
	void update();
	bool closed() const;

	inline GLFWwindow* getWindow() const { return _window; }
private:
	bool init();
};

