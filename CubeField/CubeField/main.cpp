#include <iostream>
#include <stdlib.h>

#include "Window.h"
#include "Model.h"

#include <chrono>


int main() {

	Window gameWindow("Cube Field", 1024, 768);
	glClearColor(0.2f, 0.3f, 0.8f, 1.0f);
	std::cout << "OpenGL" << glGetString(GL_VERSION) << std::endl;

	Model truck(gameWindow.getWindow(), "L200-OBJ-triangles/truck.obj", "L200-OBJ-triangles/truck_color.jpg");
	Model plane(gameWindow.getWindow(), "FREOBJ/FREOBJ2.obj", "FREOBJ/CIRRUSTS.jpg");

	while (glfwGetKey(gameWindow.getWindow(), GLFW_KEY_ESCAPE) != GLFW_PRESS && !gameWindow.closed())
	{
		gameWindow.clear();

		//=== Loop Here ===

		truck.Draw();

		plane.SetScale(glm::vec3(10.0f, 10.0f, 10.0f));
		plane.SetRotation(glm::vec3(-90.0f, -90.0f, 0.0f));
		plane.SetTranslation(glm::vec3(0.0f, 5.0f, 0.0f));
		plane.Draw();

		//=== End Loop ===

		gameWindow.update();
	}

	// Create and load objects
	//Model truck(gameWindow.window, "L200-OBJ-triangles/truck.obj", "L200-OBJ-triangles/truck_color.jpg");
	//Model plane(gameWindow.window, "FREOBJ/FREOBJ2.obj", "FREOBJ/CIRRUSTS.jpg");

	//clock_t begin = clock();
	//do {
	//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	//	clock_t end = clock();
	//	float elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;

	//	// Draw objects each frame
	//	truck.Draw();

	//	plane.SetScale(glm::vec3(10.0f, 10.0f, 10.0f));
	//	plane.SetRotation(glm::vec3(-90.0f, -90.0f, 0.0f));
	//	plane.SetTranslation(glm::vec3(0.0f, 5.0f, 0.0f));
	//	plane.Draw();


	//	// Swap buffers
	//	glfwSwapBuffers(gameWindow.window);
	//	glfwPollEvents();

	//} // Check if the ESC key was pressed or the window was closed
	//while (glfwGetKey(gameWindow.window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
	//	glfwWindowShouldClose(gameWindow.window) == 0);

	
	//system("PAUSE");
	return 0;
}