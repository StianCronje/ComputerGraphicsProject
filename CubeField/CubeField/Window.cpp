#include <iostream>
#include "Window.h"

void window_resize(GLFWwindow *window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);

Window::Window(const char* title, int width, int height)
{
	_title = title;
	_width = width;
	_height = height;
	if (!init())
	{
		glfwTerminate();
	}

	for(int i = 0; i < MAX_KEYS; i++)
	{
		_keys[i] = false;
	}

	for(int i = 0; i < MAX_BUTTONS; i++)
	{
		_mouseButtons[i] = false;
	}
}

bool Window::init()
{
	if (!glfwInit())
	{
		std::cout << "Failed to initialize GLFW" << std::endl;
		return false;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	_window = glfwCreateWindow(_width, _height, _title, NULL, NULL);
	if (!_window)
	{
		std::cout << "Failed to initialize Window" << std::endl;
			return false;

	}
	glfwMakeContextCurrent(_window);
	glfwSetWindowUserPointer(_window, this);
	glfwSetWindowSizeCallback(_window, window_resize);
	glfwSetKeyCallback(_window, key_callback);
	glfwSetMouseButtonCallback(_window, mouse_button_callback);
	glfwSetCursorPosCallback(_window, cursor_position_callback);

	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		std::cout << "Failed to initialize GLEW" << std::endl;
		return false;
	}
	/*glfwSetInputMode(_window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPos(_window, _width / 2, _height / 2);*/
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	_begin = clock();

	return true;
}

void Window::clear() const
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::update()
{
	glfwPollEvents();

	_end = clock();
	glfwSwapBuffers(_window);
}

bool Window::closed() const
{
	return glfwWindowShouldClose(_window) == 1;
}

bool Window::isKeyPressed(unsigned int keycode)
{
	if(keycode >= MAX_KEYS)
		return false;

	return _keys[keycode];
}

bool Window::isMouseButtonPressed(unsigned int button)
{
	if(button >= MAX_BUTTONS)
		return false;

	return _mouseButtons[button];
}

void Window::getMousePosition(double& x, double& y)
{
	x = _mouseX;
	y = _mouseY;
}

Window::~Window()
{
	glfwTerminate();
}

void window_resize(GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	Window* win = (Window*) glfwGetWindowUserPointer(window);
	win->_keys[key] = (action != GLFW_RELEASE);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	Window* win = (Window*) glfwGetWindowUserPointer(window);
	win->_mouseButtons[button] = (action != GLFW_RELEASE);
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	Window* win = (Window*) glfwGetWindowUserPointer(window);
	win->_mouseX = xpos;
	win->_mouseY = ypos;
}
