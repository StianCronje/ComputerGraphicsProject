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


	// std::cout << "Create window" << std::endl;
	// // Initialise GLFW
	// if (!glfwInit())
	// {
	// 	std::cout << "Failed to initialize GLFW" << std::endl;
	// }

	

	// Open a window and create its OpenGL context
	//window = glfwCreateWindow(width, height, title, NULL, NULL);
	//if (window == NULL) {
	//	std::cout << "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials." << std::endl;
	//	getchar();
	//	glfwTerminate();
	//}
	//glfwMakeContextCurrent(window);

	//// Initialize GLEW
	//glewExperimental = true; // Needed for core profile
	//if (glewInit() != GLEW_OK) {
	//	std::cout << "Failed to initialize GLEW" << std::endl;
	//	getchar();
	//	glfwTerminate();
	//}

	//// Ensure we can capture the escape key being pressed below
	//glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	//// Hide the mouse and enable unlimited mouvement
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//// Set the mouse at the center of the screen
	//glfwPollEvents();
	//glfwSetCursorPos(window, width / 2, height / 2);

	//// Enable depth test
	//glEnable(GL_DEPTH_TEST);
	//// Accept fragment if it closer to the camera than the former one
	//glDepthFunc(GL_LESS);

	//// Cull triangles which normal is not towards the camera
	//glEnable(GL_CULL_FACE);
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
	glfwSetInputMode(_window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPos(_window, _width / 2, _height / 2);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);

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
	//glfwGetFramebufferSize(_window, &_width, &_height);
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
