#include <iostream>
#include <stdlib.h>

#include "Detect.h"
#include "Window.h"
#include "Model.h"
#include "ObstacleSpawner.h"
#include "PerlinGen.h"
#include "common/controls.hpp"

#include <chrono>

void process_lighting();
void reset(Model* player, std::vector<ObstacleSpawner>& spawner);

glm::vec3 ambientColor = glm::vec3(0.2, 0.2, 0.1);
glm::vec3 diffusePosition = glm::vec3(20.0, 50.0, 0.0);
glm::vec3 diffuseColor = glm::vec3(1.0, 1.0, 1.0);
glm::vec3 diffusePosition2 = glm::vec3(0, 0, 0);
glm::vec3 diffuseColor2 = glm::vec3(0.4, 0.6, 0.95);
glm::vec3 specColor = glm::vec3(0.2, 0.2, 0.1);

std::vector<Model*> astroids;

float a = 0.0, b = 0.0, c = 0.0;
glm::vec3 cameraOffset = glm::vec3(0, 1.5, 20);
glm::vec3 cameraLookat = glm::vec3(0, 0, -20);
float startShipTurnSpeed = 0.5;
float startShipTurnAngle = 25;
float startShipMoveSpeed = 0.1;
float shipTurnSpeed;
float shipMoveSpeed;
float shipTurnAngle;

int spawn_size = 10;
int x_size = 200;
int y_size = 50;
int z_size = 100;
int blocksPassed = 0;
int currentBlockIndex = 0;

double frameStart = 0.0;
double frameEnd = 0.0;
double deltaTime = 1.0;

glm::vec3 tempVec3(0, 0, 0);

int main() {

	Window gameWindow("Astroid Field", 1024, 768);
	glClearColor(0.01f, 0.0f, 0.03f, 1.0f);
	std::cout << "OpenGL" << glGetString(GL_VERSION) << std::endl;

	Model::InitShaders();

	setCameraSpeed(0);


	Model playerShip(gameWindow.getWindow(), "Models/Ship_3.obj", "Models/Ship_tex.png");
	Model astrd1(gameWindow.getWindow(), "Models/astrd_1.obj", "Models/FireAsteroid.jpg");
	Model astrd2(gameWindow.getWindow(), "Models/astrd_2.obj", "Models/Asteroid5.png");
	Model astrd3(gameWindow.getWindow(), "Models/astrd_3.obj", "Models/BlueAsteroid.jpg");
	Model astrd4(gameWindow.getWindow(), "Models/astrd_4.obj", "Models/Asteroid8.jpg");
	astroids.push_back(&astrd1);
	astroids.push_back(&astrd2);
	astroids.push_back(&astrd3);
	astroids.push_back(&astrd4);

	x_size /= spawn_size;
	y_size /= spawn_size;
	z_size /= spawn_size;

	//ObstacleSpawner obstacles(astroids);
	//ObstacleSpawner obstacles2(astroids);
	std::vector<ObstacleSpawner> obstaclesArray{
		ObstacleSpawner(astroids),
		ObstacleSpawner(astroids),
		ObstacleSpawner(astroids)
	};
	currentBlockIndex = blocksPassed % obstaclesArray.size();
	//obstacles.SetOffset(glm::vec3(-x_size/4, 0, -z_size));
	//obstacles.Generate(spawn_size, x_size, y_size, z_size);

	reset(&playerShip, obstaclesArray);

	cameraOffset.y *= y_size;
	setCameraPosition(cameraOffset);

	while (glfwGetKey(gameWindow.getWindow(), GLFW_KEY_ESCAPE) != GLFW_PRESS && !gameWindow.closed())
	{
		gameWindow.clear();
		frameStart = glfwGetTime();


		//=== Loop Here ===
		process_lighting();

		// example on how to get a key input
		playerShip.SetRotation(glm::vec3(0.0f, 0.0f, 0.0f));

		c += shipMoveSpeed;

		if (gameWindow.isKeyPressed(GLFW_KEY_R))
			reset(&playerShip, obstaclesArray);

		if (gameWindow.isKeyPressed(GLFW_KEY_A))
		{
			std::cout << "'A' is pressed" << std::endl;
			if (b >= -47) {
				b -= shipTurnSpeed;
				playerShip.SetRotation(glm::vec3(0.0f, 0.0f, shipTurnAngle));
			}
		}
		if (gameWindow.isKeyPressed(GLFW_KEY_S))
		{
			std::cout << "'S' is pressed" << std::endl;
			if (a <= 55) {
				a += shipTurnSpeed;
				playerShip.SetRotation(glm::vec3(shipTurnAngle, 0.0f, 0.0f));
			}
		}
		if (gameWindow.isKeyPressed(GLFW_KEY_W))
		{

			std::cout << "'W' is pressed" << std::endl;
			if (a >= -17) {
				a -= shipTurnSpeed;
				playerShip.SetRotation(glm::vec3(-shipTurnAngle, 0.0f, 0.0f));
			}
		}
		if (gameWindow.isKeyPressed(GLFW_KEY_D))
		{
			std::cout << "'D' is pressed" << std::endl;
			if (b <= 47) {
				b += shipTurnSpeed;
				playerShip.SetRotation(glm::vec3(0.0f, 0.0f, -shipTurnAngle));
			}
		}

		// example on how to get mouse buttons
		if (gameWindow.isMouseButtonPressed(GLFW_MOUSE_BUTTON_1))
		{
			std::cout << "camspeed: " << getCameraSpeed() << std::endl;
			shipMoveSpeed += 0.1;
		}

		playerShip.SetScale(glm::vec3(2, 2, 2));
		playerShip.SetTranslation(glm::vec3(b, a, -c));
		tempVec3 = getCameraPosition();
		tempVec3.z = playerShip.GetTranslation().z + cameraOffset.z;
		setCameraPosition(tempVec3);
		tempVec3.z = playerShip.GetTranslation().z + cameraLookat.z;
		setCameraLookat(tempVec3);
		diffusePosition2 = getCameraPosition();
		playerShip.Draw();

		for (unsigned int i = 0; i < obstaclesArray.size(); i++)
		{
			obstaclesArray[i].Spawn();
			if (obstaclesArray[i].CheckColission(&playerShip, 10))
			{
				std::cout << "HIT ASTROID" << std::endl;
				shipMoveSpeed = 0;
				shipTurnSpeed = 0;
				shipTurnAngle = 0;
			}
		}

		// if the camera is at the end of the block
		if (getCameraPosition().z <= obstaclesArray[currentBlockIndex].GetOffset().z * spawn_size)
		{
			glm::vec3 temp = obstaclesArray[currentBlockIndex].GetOffset();
			temp.z -= obstaclesArray.size() * z_size;
			obstaclesArray[currentBlockIndex].SetOffset(temp);
			obstaclesArray[currentBlockIndex].Generate(spawn_size, x_size, y_size, z_size);

			blocksPassed++;
			currentBlockIndex = blocksPassed % obstaclesArray.size();
		}

		std::cout << "FPS: " << static_cast<int>(1 / deltaTime) << std::endl;

		//=== End Loop ===
		frameEnd = glfwGetTime();
		deltaTime = frameEnd - frameStart;
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
	GLint Diffuse_Light_position1 = glGetUniformLocation(Model::ShaderID, "lightPos1");
	glUniform3fv(Diffuse_Light_position1, 1, glm::value_ptr(diffusePosition2));
	// Set the Diffuse light Position
	GLint Diffuse_Light_color = glGetUniformLocation(Model::ShaderID, "lightColor");
	glUniform3fv(Diffuse_Light_color, 1, glm::value_ptr(diffuseColor));
	GLint Diffuse_Light_color1 = glGetUniformLocation(Model::ShaderID, "lightColor1");
	glUniform3fv(Diffuse_Light_color1, 1, glm::value_ptr(diffuseColor2));

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

void reset(Model* player, std::vector<ObstacleSpawner>& spawner)
{
	// reset player pos and input
	a = b = c = 0.0;
	player->SetTranslation(glm::vec3(b, a, -c));
	shipTurnSpeed = startShipTurnSpeed;
	shipMoveSpeed = startShipMoveSpeed;
	shipTurnAngle = startShipTurnAngle;

	// reset obstacle pos
	for (int i = 0; i < spawner.size(); i++)
	{

		spawner[i].SetOffset(glm::vec3(-x_size / 4, 0, -z_size * (i + 1)));
		spawner[i].Generate(spawn_size, x_size, y_size, z_size);
		std::cout << "Spawn Block at Z: " << spawner[i].GetOffset().z << std::endl;
	}

}