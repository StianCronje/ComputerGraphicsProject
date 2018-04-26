#pragma once
#include "glm/glm.hpp"
#include <vector>
#include <iostream>

#include "Model.h"
#include "PerlinGen.h"

class ObstacleSpawner
{
private:
	PerlinGen generator;
	float** spawnMatrix;
	glm::vec3 _offset = glm::vec3(0, 0, 0);
	std::vector<Model*> _obstacleModels;
	std::vector<glm::vec3> _obstaclePos;
	std::vector<glm::vec3> _obstacleRot;
	std::vector<int> _modelAtPos;
	int _spawnSize = 100;
public:
	ObstacleSpawner(std::vector<Model*> &obstacleModels);
	~ObstacleSpawner();

	void Generate(int size, int x_space, int y_space, int z_space);
	void Spawn();
	bool CheckColission(Model* player, float dist);
	inline void SetOffset(glm::vec3 offset) { _offset = offset; }
};

