#include <iostream>
#include <stdlib.h>

#include "Detect.h"
#include "Window.h"
#include "Model.h"
#include "ObstacleSpawner.h"
#include "PerlinGen.h"
#include "common/controls.hpp"
#include "common/imgui.h"
#include "common/imgui_impl_glfw_gl3.h"

#include <chrono>

// functions

void reset(Model *player, std::vector<ObstacleSpawner> &spawner);
void crash(Model *player);



// LightingVars

glm::vec3 dirLightDir = glm::vec3(-0.4f, 0.0f, 0.0f);
glm::vec3 dirLightColor = glm::vec3(0.95f, 0.95f, 0.77f);
glm::vec3 ambientColor = glm::vec3(0.6f);
glm::vec3 cameraLightPos = glm::vec3(0, 0, 0);
glm::vec3 cameraLightColor = glm::vec3(1.0, 1.0, 1.0);
glm::vec3 explosionLightPos = glm::vec3(0, 0, 0);
glm::vec3 explosionLightColor = glm::vec3(1.0, 0.6, 0.0);
glm::vec3 specColor = glm::vec3(0.2, 0.2, 0.1);

glm::vec3 crashPos = glm::vec3(0, 0, 0);
glm::vec3 startExplosionLightPos = glm::vec3(0, 0, 0);
glm::vec3 offExplosionLightColor = glm::vec3(0, 0, 0);
glm::vec3 onExplosionLightColor = glm::vec3(1.0, 0.6, 0.0);

// Variables

std::vector<Model *> astroids;

float a = 0.0, b = 0.0, c = 0.0;
glm::vec3 cameraOffset = glm::vec3(0, 0, 20);
glm::vec3 cameraLookat = glm::vec3(0, 0, -2);
float offsetDamp = 0.1f;
float lookatDamp = 0.2f;
float startShipTurnSpeed = 0.5;
float startShipTurnAngle = 25;
float startShipMoveSpeed = 0.1;
float shipTurnSpeed;
float shipMoveSpeed;
float shipTurnAngle;
bool crashed = false;

int spawn_count_x = 10;
int spawn_count_y = 10;
int x_size = 200;
int y_size = 50;
int z_size = 100;
int blocksPassed = 0;
int currentBlockIndex = 0;

double frameStart = 0.0;
double frameEnd = 0.0;
double deltaTime = 1.0;

glm::vec3 tempVec3(0, 0, 0);

bool menuOpen = true;

int score = 0;
float scoreMult = 1.0f;
int speedIncrInterval = 50;
float speedIncr = 0.01f;

int main()
{

	// Setup Window

	Window gameWindow("Astroid Field", 1024, 768);
	glClearColor(0.01f, 0.0f, 0.03f, 1.0f);
	std::cout << "OpenGL" << glGetString(GL_VERSION) << std::endl;

	// Setup UI
	ImGui::CreateContext();
	ImGuiIO &io = ImGui::GetIO();
	(void)io;
	ImGui_ImplGlfwGL3_Init(gameWindow.getWindow(), true);
	ImGui::StyleColorsDark();

	// Setup Lighting

	Model::directionalLightDir = dirLightDir;
	Model::directionalLightColor = dirLightColor;
	Model::ambientColor = ambientColor;
	Model::cameraLightPos = cameraLightPos;
	Model::cameraLightColor = cameraLightColor;
	Model::explosionLightPos = explosionLightPos;
	Model::explosionLightColor = explosionLightColor;

	// Load Models

	Model playerShip(gameWindow.getWindow(), "Models/Ship_3.obj", "Models/Ship_tex.png");
	Model astrd1(gameWindow.getWindow(), "Models/astrd_1.obj", "Models/FireAsteroid.jpg");
	Model astrd2(gameWindow.getWindow(), "Models/astrd_2.obj", "Models/Asteroid5.png");
	Model astrd3(gameWindow.getWindow(), "Models/astrd_3.obj", "Models/BlueAsteroid.jpg");
	Model astrd4(gameWindow.getWindow(), "Models/astrd_4.obj", "Models/Asteroid8.jpg");

	// Populate Astroid Pool
	Model background(gameWindow.getWindow(), "Models/background.obj", "Models/background.jpg");
	astroids.push_back(&astrd1);
	astroids.push_back(&astrd2);
	astroids.push_back(&astrd3);
	astroids.push_back(&astrd4);

	x_size /= spawn_count_x;
	y_size /= spawn_count_x;
	z_size /= spawn_count_y;

	std::vector<ObstacleSpawner> obstaclesArray{
		ObstacleSpawner(astroids),
		ObstacleSpawner(astroids),
		ObstacleSpawner(astroids)};
	currentBlockIndex = blocksPassed % obstaclesArray.size();

	astroids.push_back(&astrd1);
	astroids.push_back(&astrd2);
	astroids.push_back(&astrd3);
	astroids.push_back(&astrd4);

	reset(&playerShip, obstaclesArray);

	// Setup Camera

	setCameraSpeed(0);
	cameraOffset.y *= y_size;
	setCameraPosition(cameraOffset);
	setCameraLookat(cameraLookat);

	while (glfwGetKey(gameWindow.getWindow(), GLFW_KEY_ESCAPE) != GLFW_PRESS && !gameWindow.closed())
	{
		gameWindow.clear();
		frameStart = glfwGetTime();

		//=== Loop Here ===
		
		if (gameWindow.isKeyPressed(GLFW_KEY_R) || io.KeysDown[GLFW_KEY_R])
			reset(&playerShip, obstaclesArray);


		if (!crashed)
		{
			playerShip.SetRotation(glm::vec3(0.0f, 0.0f, 0.0f));
			c += shipMoveSpeed;

			// Handle Input
			if (gameWindow.isKeyPressed(GLFW_KEY_A) || io.KeysDown[GLFW_KEY_A])
			{
				std::cout << "'A' is pressed" << std::endl;
				if (b >= -47)
				{
					b -= shipTurnSpeed;
					playerShip.SetRotation(glm::vec3(0.0f, 0.0f, shipTurnAngle));
				}
			}
			if (gameWindow.isKeyPressed(GLFW_KEY_S) || io.KeysDown[GLFW_KEY_S])
			{
				std::cout << "'S' is pressed" << std::endl;
				if (a <= 55)
				{
					a += shipTurnSpeed;
					playerShip.SetRotation(glm::vec3(shipTurnAngle, 0.0f, 0.0f));
				}
			}
			if (gameWindow.isKeyPressed(GLFW_KEY_W) || io.KeysDown[GLFW_KEY_W])
			{

				std::cout << "'W' is pressed" << std::endl;
				if (a >= -17)
				{
					a -= shipTurnSpeed;
					playerShip.SetRotation(glm::vec3(-shipTurnAngle, 0.0f, 0.0f));
				}
			}
			if (gameWindow.isKeyPressed(GLFW_KEY_D) || io.KeysDown[GLFW_KEY_D])
			{
				std::cout << "'D' is pressed" << std::endl;
				if (b <= 47)
				{
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

			score = c * scoreMult;

			if (score % speedIncrInterval == 0)
				shipMoveSpeed += speedIncr;
		}

		// Update Player and Camera

		playerShip.SetScale(glm::vec3(2, 2, 2));
		playerShip.SetTranslation(glm::vec3(b, a, -c));
		background.SetScale(glm::vec3(500, 500, 5000));
		background.SetTranslation(glm::vec3(0, 0, 0));
		background.SetRotation(glm::vec3(90, 0, 0));
		// Model::cameraLightPos = playerShip.GetTranslation();
		
		tempVec3 = playerShip.GetTranslation() + cameraOffset;
		tempVec3.y *= offsetDamp;
		tempVec3.x *= offsetDamp;
		setCameraPosition(tempVec3);
		std::cout << Model::cameraLightPos.x << ", " << Model::cameraLightPos.y << ", " << Model::cameraLightPos.z << std::endl;
		tempVec3 = playerShip.GetTranslation() + cameraLookat;
		tempVec3.y *= lookatDamp;
		tempVec3.x *= lookatDamp;
		setCameraLookat(tempVec3);
		background.Draw();
		playerShip.Draw();

		// Check Collisions

		for (unsigned int i = 0; i < obstaclesArray.size(); i++)
		{
			obstaclesArray[i].Spawn();
			if (obstaclesArray[i].CheckColission(&playerShip, 10))
			{
				std::cout << "HIT ASTROID" << std::endl;
				crash(&playerShip);
			}
		}

		// Update Astroid Blocks

		// if the camera is at the end of the block
		if (getCameraPosition().z <= obstaclesArray[currentBlockIndex].GetOffset().z * spawn_count_y)
		{
			glm::vec3 temp = obstaclesArray[currentBlockIndex].GetOffset();
			temp.z -= obstaclesArray.size() * z_size;
			obstaclesArray[currentBlockIndex].SetOffset(temp);
			obstaclesArray[currentBlockIndex].Generate(spawn_count_x, spawn_count_y, x_size, y_size, z_size);

			blocksPassed++;
			currentBlockIndex = blocksPassed % obstaclesArray.size();
		}

		// Render UI
		ImGui_ImplGlfwGL3_NewFrame();
		ImGui::Begin("", &menuOpen, ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoTitleBar);
		ImGui::Text("Score: %d", score);
		if(crashed)
		{
			if(ImGui::Button("Reset"))
			{
				std::cout << "reseted" << std::endl;
				reset(&playerShip, obstaclesArray);
			}
		}
		ImGui::End();
		ImGui::Render();
		ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

		std::cout << "FPS: " << static_cast<int>(1 / deltaTime) << std::endl;

		//=== End Loop ===
		frameEnd = glfwGetTime();
		deltaTime = frameEnd - frameStart;
		gameWindow.update();
	}

	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();

	return 0;
}

void reset(Model *player, std::vector<ObstacleSpawner> &spawner)
{
	// reset player pos and input
	a = b = c = 0.0;
	player->SetTranslation(glm::vec3(b, a, -c));
	shipTurnSpeed = startShipTurnSpeed;
	shipMoveSpeed = startShipMoveSpeed;
	shipTurnAngle = startShipTurnAngle;

	//reset crash light
	Model::explosionLightColor = offExplosionLightColor;
	Model::explosionLightPos = startExplosionLightPos;

	// reset obstacle pos
	blocksPassed = 0;
	for (int i = 0; i < spawner.size(); i++)
	{
		spawner[i].SetOffset(glm::vec3(-x_size / 4, 0, -z_size * (i + 1)));
		spawner[i].Generate(spawn_count_x, spawn_count_y, x_size, y_size, z_size);
		std::cout << "Spawn Block at Z: " << spawner[i].GetOffset().z << std::endl;
	}
	crashed = false;
}

void crash(Model *player)
{
	crashed = true;
	Model::explosionLightPos = player->GetTranslation() + glm::vec3(0, 1, 0);
	Model::explosionLightColor = onExplosionLightColor * glm::vec3(2, 2, 2);
	shipMoveSpeed = 0;
	shipTurnSpeed = 0;
	shipTurnAngle = 0;
}