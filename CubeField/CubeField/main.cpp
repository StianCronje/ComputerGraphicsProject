#include <iostream>
#include <stdlib.h>
#include <cstdlib>

#include "Detect.h"
#include "Window.h"
#include "Model.h"
#include "common/controls.hpp"

#include <chrono>

#define SPAWN_SIZE 500

void process_lighting();

glm::vec3 ambientColor = glm::vec3(0.0, .0, 0.0);
glm::vec3 diffusePosition = glm::vec3(20.0, 50.0, 0.0);
glm::vec3 diffuseColor = glm::vec3(1.0, 1.0, 1.0);
glm::vec3 specColor = glm::vec3(0.0, 0.0, 0.0);

int main() {

	Window gameWindow("Cube Field", 1024, 768);
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	std::cout << "OpenGL" << glGetString(GL_VERSION) << std::endl;

	// Model::InitShaders();

	Model truck(gameWindow.getWindow(), "L200-OBJ-triangles/truck.obj", "L200-OBJ-triangles/truck_color.jpg");
	Model truck2(gameWindow.getWindow(), "L200-OBJ-triangles/truck.obj", "L200-OBJ-triangles/truck_color.jpg");
	Model plane(gameWindow.getWindow(), "FREOBJ/FREOBJ2.obj", "FREOBJ/CIRRUSTS.jpg");
	float pos = 0.0f; 
	float rot = 0.0f;
	float mov = 0.0f;
	truck.SetRotation(glm::vec3(0.0f, 180.0f, 0.5f));

	Model::InitShaders();

	Model playerShip(gameWindow.getWindow(), "Models/Ship_3.obj", "Models/Ship_tex.png");
	Model astrd1(gameWindow.getWindow(), "Models/astrd_1.obj", "L200-OBJ-triangles/truck_color.jpg");
	Model astrd2(gameWindow.getWindow(), "Models/astrd_2.obj", "L200-OBJ-triangles/truck_color.jpg");
	Model astrd3(gameWindow.getWindow(), "Models/astrd_3.obj", "L200-OBJ-triangles/truck_color.jpg");
	Model astrd4(gameWindow.getWindow(), "Models/astrd_4.obj", "L200-OBJ-triangles/truck_color.jpg");


	std::vector<glm::vec3> obstaclePos;
	std::vector<glm::vec3> obstacleRot;
	for (int i = 0; i < 50; i++)
	{
		//TODO: replace this with actual rand position generator
		obstaclePos.push_back(glm::vec3((rand() % SPAWN_SIZE) - (SPAWN_SIZE / 2), 0.0f, -(rand() % SPAWN_SIZE) + (SPAWN_SIZE / 5)));
		obstacleRot.push_back(glm::vec3(rand() % 360, rand() % 360, rand() % 360));
	}

	while (glfwGetKey(gameWindow.getWindow(), GLFW_KEY_ESCAPE) != GLFW_PRESS && !gameWindow.closed())
	{
		
		gameWindow.clear();
		mov-=0.2f;
		//=== Loop Here ===
		process_lighting();		

		// example on how to get a key input
		if(gameWindow.isKeyPressed(GLFW_KEY_A))
		{
			pos++;
		}
		if (gameWindow.isKeyPressed(GLFW_KEY_D))
		{
			pos--;
		}
		if (gameWindow.isKeyPressed(GLFW_KEY_W))
		{
			rot++;
			truck.SetRotation(glm::vec3(0.0f, rot, 0.0f));
		}
		if (gameWindow.isKeyPressed(GLFW_KEY_S))
		{
			rot--;
			truck.SetRotation(glm::vec3(0.0f, rot, 0.0f));
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

		//playerShip.SetScale(glm::vec3(5.0f, 5.0f, 5.0f));
		playerShip.Draw();

#pragma region Draw Astroids
		for (int i = 0; i < obstaclePos.size(); i++)
		{
			switch (i % 3)
			{
			case 0:
				astrd1.SetTranslation(obstaclePos[i]);
				astrd1.SetRotation(obstacleRot[i]);
				astrd1.Draw();
				break;
			case 1:
				astrd2.SetTranslation(obstaclePos[i]);
				astrd2.SetRotation(obstacleRot[i]);
				astrd2.Draw();
				break;
			case 2:
				astrd3.SetTranslation(obstaclePos[i]);
				astrd3.SetRotation(obstacleRot[i]);
				astrd3.Draw();
				break;
			default:
				astrd4.SetTranslation(obstaclePos[i]);
				astrd4.SetRotation(obstacleRot[i]);
				astrd4.Draw();
				break;
			}
		}
#pragma endregion

		

		//=== End Loop ===

		gameWindow.update();
	}
	
	//system("PAUSE");
	return 0;
}

void process_lighting()
{
	
	// Set the ambient light color
		GLint Ambient_Light_color = glGetUniformLocation(Model::ShaderID, "ambientColor");
		glUniform3fv(Ambient_Light_color, 1, glm::value_ptr(ambientColor));
		GLint Ambient_Light_strength = glGetUniformLocation(Model::ShaderID, "ambientStrength");
		glUniform1f(Ambient_Light_strength, 0.2);

		//Diffuse Staff
		// Set the Diffuse light Position
		GLint Diffuse_Light_position = glGetUniformLocation(Model::ShaderID, "lightPos");
		glUniform3fv(Diffuse_Light_position, 1, glm::value_ptr(diffusePosition));
		// GLint Diffuse_Light_position1 = glGetUniformLocation(Model::ShaderID, "lightPos1");
		// glUniform3fv(Diffuse_Light_position1, 1, glm::value_ptr(glm::vec3(40.0, 0.0, 0.0)));
		// GLint Diffuse_Light_position2 = glGetUniformLocation(Model::ShaderID, "lightPos2");
		// glUniform3fv(Diffuse_Light_position2, 1, glm::value_ptr(glm::vec3(-40.0, 0.0, 0.0)));
		// Set the Diffuse light Position
		GLint Diffuse_Light_color = glGetUniformLocation(Model::ShaderID, "lightColor");
		glUniform3fv(Diffuse_Light_color, 1, glm::value_ptr(diffuseColor));
		// GLint Diffuse_Light_color1 = glGetUniformLocation(Model::ShaderID, "lightColor1");
		// glUniform3fv(Diffuse_Light_color1, 1, glm::value_ptr(glm::vec3(1.0, 0.0, 0.0)));
		// GLint Diffuse_Light_color2 = glGetUniformLocation(Model::ShaderID, "lightColor2");
		// glUniform3fv(Diffuse_Light_color2, 1, glm::value_ptr(glm::vec3(0.0, 0.0, 1.0)));


		//Specular Staff
		//Set the Cameraposition (eye location)
		GLint eye_location = glGetUniformLocation(Model::ShaderID, "vertexPosition_cameraspace");
		glm::vec3 CameraPosition = getCameraPosition();
		glUniform3fv(eye_location, 1, glm::value_ptr(CameraPosition));
		//Set the specular color
		GLint Spec_Light_color = glGetUniformLocation(Model::ShaderID, "specColor");
		glUniform3fv(Spec_Light_color, 1, glm::value_ptr(diffuseColor));
		// Set the Specular light Position
		GLint Spec_Light_position = glGetUniformLocation(Model::ShaderID, "speclightPos");
		glUniform3fv(Spec_Light_position, 1, glm::value_ptr(diffusePosition));
}