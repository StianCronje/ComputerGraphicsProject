#pragma once

#include <GL/glew.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <GLFW/glfw3.h>
#include <chrono>


class Window
{
private:
	const char* _title;
	int _width, _height;
	GLFWwindow* _window;
	clock_t _begin;
	clock_t _end;
public:
	Window(const char* title, int width, int height);
	~Window();
	bool init();
	void clear() const;
	void update();
	bool closed() const;

	inline GLFWwindow* getWindow() const { return _window; }
	inline float getTime() const { return double(_end - _begin) / CLOCKS_PER_SEC; }
private:
};

