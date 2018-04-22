#pragma once

#include <GL/glew.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <GLFW/glfw3.h>
#include <chrono>

#define MAX_KEYS 1024
#define MAX_BUTTONS 1024

class Window
{
private:
	const char* _title;
	int _width, _height;
	GLFWwindow* _window;
	clock_t _begin;
	clock_t _end;

	bool _keys[MAX_KEYS];
	bool _mouseButtons[MAX_BUTTONS];
	double _mouseX, _mouseY;
public:
	Window(const char* title, int width, int height);
	~Window();
	bool init();
	void clear() const;
	void update();
	bool closed() const;

	inline GLFWwindow* getWindow() const { return _window; }
	inline float getTime() const { return double(_end - _begin) / CLOCKS_PER_SEC; }

	bool isKeyPressed(unsigned int keycode);
	bool isMouseButtonPressed(unsigned int button);
	void getMousePosition(double& x, double& y);
private:
	friend void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	friend void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
	friend void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
};

