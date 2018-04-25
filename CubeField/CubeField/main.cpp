#include <iostream>
#include <stdlib.h>

#include "Detect.h"
#include "Window.h"
#include "Model.h"

#include <chrono>


int main() {

	Window gameWindow("Cube Field", 1024, 768);
	glClearColor(0.2f, 0.3f, 0.8f, 1.0f);
	std::cout << "OpenGL" << glGetString(GL_VERSION) << std::endl;

	Model truck(gameWindow.getWindow(), "L200-OBJ-triangles/truck.obj", "L200-OBJ-triangles/truck_color.jpg");
	Model truck2(gameWindow.getWindow(), "L200-OBJ-triangles/truck.obj", "L200-OBJ-triangles/truck_color.jpg");
	Model plane(gameWindow.getWindow(), "FREOBJ/FREOBJ2.obj", "FREOBJ/CIRRUSTS.jpg");
	float pos = 0.0f; 
	float rot = 0.0f;
	float mov = 0.0f;
	truck.SetRotation(glm::vec3(0.0f, 180.0f, 0.5f));
	while (glfwGetKey(gameWindow.getWindow(), GLFW_KEY_ESCAPE) != GLFW_PRESS && !gameWindow.closed())
	{
		
		gameWindow.clear();
		mov-=0.2f;
		//=== Loop Here ===
		
		// example on how to get a key input
		if(gameWindow.isKeyPressed(GLFW_KEY_A))
		{
			pos++;
			//std::cout << "'A' is pressed" << std::endl;
			//truck.SetTranslation(glm::vec3(pos, 5.0f, 0.0f));
		//	truck.Draw();
		}
		if (gameWindow.isKeyPressed(GLFW_KEY_D))
		{
			pos--;
		//	std::cout << "'A' is pressed" << std::endl;
		//	truck.SetTranslation(glm::vec3(pos, 5.0f, 0.0f));
		//	truck.Draw();
		}
		if (gameWindow.isKeyPressed(GLFW_KEY_W))
		{
			rot++;
			//std::cout << "'A' is pressed" << std::endl;
			truck.SetRotation(glm::vec3(0.0f, rot, 0.0f));
			//truck.Draw();
		}
		if (gameWindow.isKeyPressed(GLFW_KEY_S))
		{
			rot--;
			//	std::cout << "'A' is pressed" << std::endl;
			truck.SetRotation(glm::vec3(0.0f, rot, 0.0f));
			//truck.Draw();
		}
		// example on how to get mouse buttons
		if(gameWindow.isMouseButtonPressed(GLFW_MOUSE_BUTTON_1))
		{
			std::cout << "'LMB' is pressed" << std::endl;			
		}
		Detect stuff(&truck, &truck2);
		if (stuff.compare(50.0))
			std::cout << "matches\n";
		// example how tp track the mouse position;
		double x, y;
		gameWindow.getMousePosition(x, y);
		//std::cout << "Mouse Position: (" << x << ", " << y << ")" << std::endl;
		glm::lookAt(glm::vec3(0.0f, 0.0f, mov), glm::vec3(0.0f, 0.0f, mov), glm::vec3(0.0f, 0.0f, mov));
		truck.SetTranslation(glm::vec3(pos, 0.0f, mov));
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