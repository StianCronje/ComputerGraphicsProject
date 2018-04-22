#include <iostream>
#include <stdlib.h>
#include <cstdlib>

#include "Window.h"
#include "Model.h"

#include <chrono>

#define SPAWN_SIZE 500


int main() {

	Window gameWindow("Cube Field", 1024, 768);
	glClearColor(0.2f, 0.3f, 0.8f, 1.0f);
	std::cout << "OpenGL" << glGetString(GL_VERSION) << std::endl;

	Model::InitShaders();

	Model plane(gameWindow.getWindow(), "FREOBJ/FREOBJ2.obj", "FREOBJ/CIRRUSTS.jpg");
	Model playerShip(gameWindow.getWindow(), "Models/Ship_3.obj", "FREOBJ/CIRRUSTS.jpg");
	Model obstacle(gameWindow.getWindow(), "Models/astrd_1.obj", "L200-OBJ-triangles/truck_color.jpg");
	Model astrd1(gameWindow.getWindow(), "Models/astrd_1.obj", "L200-OBJ-triangles/truck_color.jpg");
	Model astrd2(gameWindow.getWindow(), "Models/astrd_2.obj", "L200-OBJ-triangles/truck_color.jpg");
	Model astrd3(gameWindow.getWindow(), "Models/astrd_3.obj", "L200-OBJ-triangles/truck_color.jpg");
	Model astrd4(gameWindow.getWindow(), "Models/astrd_4.obj", "L200-OBJ-triangles/truck_color.jpg");

	std::vector<glm::vec3> obstaclePos;
	for (int i = 0; i < 50; i++)
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
				
		playerShip.SetScale(glm::vec3(5.0f, 5.0f, 5.0f));
		playerShip.Draw();

		/*plane.SetScale(glm::vec3(10.0f, 10.0f, 10.0f));
		plane.SetRotation(glm::vec3(-90.0f, gameWindow.getTime() * -10.0f, 0.0f));
		plane.SetTranslation(glm::vec3(0.0f, 5.0f, 0.0f));
		plane.Draw();*/

		for (int i = 0; i < obstaclePos.size(); i++)
		{
			switch (i % 3)
			{
			case 0:
				astrd1.SetTranslation(obstaclePos[i]);
				astrd1.Draw();
				break;
			case 1:
				astrd2.SetTranslation(obstaclePos[i]);
				astrd2.Draw();
				break;
			case 2:
				astrd3.SetTranslation(obstaclePos[i]);
				astrd3.Draw();
				break;
			default:
				astrd4.SetTranslation(obstaclePos[i]);
				astrd4.Draw();
				break;
			}
		}

		//=== End Loop ===

		gameWindow.update();
	}
	
	//system("PAUSE");
	return 0;
}