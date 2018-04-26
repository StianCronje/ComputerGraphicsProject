#include "ObstacleSpawner.h"
#include "Detect.h"

ObstacleSpawner::ObstacleSpawner(std::vector<Model*> &obstacleModels)
{
	_obstacleModels = obstacleModels;
}

void ObstacleSpawner::Generate(int size, int x_scale, int y_scale, int z_scale)
{
	generator.perlinSetValues(rand(), 1);
	generator.setMatrixSize(size);
	generator.popVertexArray();
	spawnMatrix = generator.getVertexArray();

	for (int y = 0; y < size; y++)
	{
		for (int x = 0; x < size; x++)
		{
			_obstaclePos.push_back(glm::vec3((x + _offset.x) * x_scale, (spawnMatrix[x][y] + _offset.y) * y_scale, (y + _offset.z) * z_scale));
			//_obstaclePos.push_back(glm::vec3(x * x_scale, spawnMatrix[x][y] * y_scale, y * z_scale) + _offset);
			_obstacleRot.push_back(glm::vec3(rand() % 360, rand() % 360, rand() % 360));
		}
	}
}



void ObstacleSpawner::Spawn()
{
	for (unsigned int i = 0; i < _obstaclePos.size(); i++)
	{
		int m = i % (_obstacleModels.size() - 1);
		_modelAtPos.push_back(m);
		(*_obstacleModels[m]).SetTranslation(_obstaclePos[i]);
		(*_obstacleModels[m]).SetRotation(_obstacleRot[i]);
		(*_obstacleModels[m]).Draw();
	}
}

bool ObstacleSpawner::CheckColission(Model* player, float dist)
{
	std::array<glm::vec3, 2> bounds;
	for (unsigned int i = 0; i < _obstaclePos.size(); i++)
	{
		if (glm::distance<float>(player->GetTranslation(), _obstaclePos[i]) <= dist)
		{
			bounds[0] = (*_obstacleModels[_modelAtPos[i]]).GetMinBounds() + _obstaclePos[i];
			bounds[1] = (*_obstacleModels[_modelAtPos[i]]).GetMaxBounds() + _obstaclePos[i];

			Detect col(player, bounds);
			if (col.comapre_bounds())
				return true;
		}
	}

	return false;
}


ObstacleSpawner::~ObstacleSpawner()
{
	_obstaclePos.clear();
	_obstacleRot.clear();
}
