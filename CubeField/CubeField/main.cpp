#include <iostream>
#include <stdlib.h>
#include <cstdlib>

#include "Window.h"
#include "Model.h"

#include "glm\gtc\random.hpp"

#include <chrono>

#define SPAWN_SIZE 100


int main() {

	Window gameWindow("Cube Field", 1024, 768);
	glClearColor(0.2f, 0.3f, 0.8f, 1.0f);
	std::cout << "OpenGL" << glGetString(GL_VERSION) << std::endl;

	Model::InitShaders();

	//Model truck(gameWindow.getWindow(), "L200-OBJ-triangles/truck.obj", "L200-OBJ-triangles/truck_color.jpg");
	Model plane(gameWindow.getWindow(), "FREOBJ/FREOBJ2.obj", "FREOBJ/CIRRUSTS.jpg");
	Model playerShip(gameWindow.getWindow(), "Models/Ship_3.obj", "FREOBJ/CIRRUSTS.jpg");
	Model obstacle(gameWindow.getWindow(), "L200-OBJ-triangles/truck.obj", "L200-OBJ-triangles/truck_color.jpg");

	std::vector<glm::vec3> obstaclePos;
	for (int i = 0; i < 10; i++)
	{
		//TODO: replace this with actual rand position generator
		obstaclePos.push_back(glm::vec3((rand() % SPAWN_SIZE) - (SPAWN_SIZE / 2), 0.0f, (rand() % SPAWN_SIZE) - (SPAWN_SIZE / 2)));
	}

	while (glfwGetKey(gameWindow.getWindow(), GLFW_KEY_ESCAPE) != GLFW_PRESS && !gameWindow.closed())
	{
		gameWindow.clear();

		//=== Loop Here ===
		
		// example on how to get a key input
		if(gameWindow.isKeyPressed(GLFW_KEY_A))
		{
			std::cout << "'A' is pressed" << std::endl;
		}

		// example on how to get mouse buttons
		if(gameWindow.isMouseButtonPressed(GLFW_MOUSE_BUTTON_1))
		{
			std::cout << "'LMB' is pressed" << std::endl;			
		}

		// example how tp track the mouse position;
		double x, y;
		gameWindow.getMousePosition(x, y);
		std::cout << "Mouse Position: (" << x << ", " << y << ")" << std::endl;

		//truck.SetTranslation(glm::vec3(20, 0, 0));
		//truck.Draw();
		
		playerShip.SetScale(glm::vec3(5.0f, 5.0f, 5.0f));
		playerShip.Draw();

		/*plane.SetScale(glm::vec3(10.0f, 10.0f, 10.0f));
		plane.SetRotation(glm::vec3(-90.0f, gameWindow.getTime() * -10.0f, 0.0f));
		plane.SetTranslation(glm::vec3(0.0f, 5.0f, 0.0f));
		plane.Draw();*/

		for (int i = 0; i < obstaclePos.size(); i++)
		{
			obstacle.SetScale(glm::vec3(0.2f, 0.2f, 0.2f));
			obstacle.SetTranslation(obstaclePos[i]);
			obstacle.Draw();
		}

		//=== End Loop ===

		gameWindow.update();
	}
	
	//system("PAUSE");
	return 0;
}