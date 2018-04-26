#include <iostream>
#include <stdlib.h>

#include "Detect.h"
#include "Window.h"
#include "Model.h"
#include "ObstacleSpawner.h"
#include "PerlinGen.h"
#include "common/controls.hpp"

#include <chrono>

#define SPAWN_SIZE 500

void process_lighting();
void fillVector(std::vector<Model>&);

glm::vec3 ambientColor = glm::vec3(0.1, 0.1, 0.1);
glm::vec3 diffusePosition = glm::vec3(20.0, 50.0, 0.0);
glm::vec3 diffuseColor = glm::vec3(1.0, 1.0, 1.0);
glm::vec3 specColor = glm::vec3(0.0, 0.0, 0.0);

std::vector<Model*> astroids;

int main() {

	Window gameWindow("Cube Field", 1024, 768);
	glClearColor(0.01f, 0.0f, 0.03f, 1.0f);
	std::cout << "OpenGL" << glGetString(GL_VERSION) << std::endl;

	Model::InitShaders();

	float a = 0.0, b = 0.0, c = 0.0;

	Model playerShip(gameWindow.getWindow(), "Models/Ship_3.obj", "Models/Ship_tex.png");
	Model astrd1(gameWindow.getWindow(), "Models/astrd_1.obj", "Models/FireAsteroid.jpg");
	Model astrd2(gameWindow.getWindow(), "Models/astrd_2.obj", "Models/Asteroid5.png");
	Model astrd3(gameWindow.getWindow(), "Models/astrd_3.obj", "Models/BlueAsteroid.jpg");
	Model astrd4(gameWindow.getWindow(), "Models/astrd_4.obj", "Models/Asteroid8.jpg");
	astroids.push_back(&astrd1);
	astroids.push_back(&astrd2);
	astroids.push_back(&astrd3);
	astroids.push_back(&astrd4);

	int spawn_size = 10;
	int x_size = 200;
	int y_size = 50;
	int z_size = 100;

	x_size /= spawn_size;
	y_size /= spawn_size;
	z_size /= spawn_size;

	ObstacleSpawner obstacles(astroids);
	//TODO: figure out proper offset
	obstacles.SetOffset(glm::vec3(-x_size/4, 0, -z_size));
	obstacles.Generate(spawn_size, x_size, y_size, z_size);

	while (glfwGetKey(gameWindow.getWindow(), GLFW_KEY_ESCAPE) != GLFW_PRESS && !gameWindow.closed())
	{		
		gameWindow.clear();
		//=== Loop Here ===
		process_lighting();		

		// example on how to get a key input
		c += 0.1;
		if (gameWindow.isKeyPressed(GLFW_KEY_A))
		{
			std::cout << "'A' is pressed" << std::endl;
			if (b >= -47) {
				b -= 1;
				playerShip.SetRotation(glm::vec3(0.0f, 0.0f, 25.0f));
			}
		}
		else if (gameWindow.isKeyPressed(GLFW_KEY_W))
		{
			std::cout << "'W' is pressed" << std::endl;
			if (a <= 55) {
				a += 1;
				playerShip.SetRotation(glm::vec3(25.0f, 0.0f, 0.0f));
			}
		}
		else if (gameWindow.isKeyPressed(GLFW_KEY_S))
		{
			
			std::cout << "'S' is pressed" << std::endl;
			if (a >= -17) {
				a -= 1;
				playerShip.SetRotation(glm::vec3(-25.0f, 0.0f, 0.0f));
			}
		}
		else if (gameWindow.isKeyPressed(GLFW_KEY_D))
		{
			std::cout << "'D' is pressed" << std::endl;
			if (b <= 47) {
				b += 1;
				playerShip.SetRotation(glm::vec3(0.0f, 0.0f, -25.0f));
			}
		}
		// example on how to get mouse buttons
		else if (gameWindow.isMouseButtonPressed(GLFW_MOUSE_BUTTON_1))
		{
			std::cout << "'LMB' is pressed" << std::endl;
		}
		else
			playerShip.SetRotation(glm::vec3(0.0f, 0.0f, 0.0f));

		// example how tp track the mouse position;
		double x, y;
		gameWindow.getMousePosition(x, y);

		playerShip.SetScale(glm::vec3(2, 2, 2));
		playerShip.SetTranslation(glm::vec3(b, a, -c));
		playerShip.Draw();

		obstacles.Spawn();
		if (obstacles.CheckColission(&playerShip, 10))
			std::cout << "HIT ASTROID" << std::endl;
				

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
