#include "GameStateManager.h"

#include <iostream>

GameStateManager::GameStateManager()
{
}


GameStateManager::~GameStateManager()
{
}

GameStateManager* GameStateManager::getInstance(){
	static GameStateManager instance;
	return &instance;
}

void GameStateManager::setPostProcessingEnabled(bool postProcessingEnabled)
{
	this->postProcessingEnabled = postProcessingEnabled;
}

bool GameStateManager::getPostProcessingEnabled()
{
	return postProcessingEnabled;
}

void GameStateManager::updatePlayerPosition(glm::vec3 position)
{
	if (position.x < 4 && position.x > 0)
	{
		if (position.z < 0 && position.z > -5)
		{
			playerPosition = ROOM_1;
		}
		else if (position.z < -5.4)
		{
			playerPosition = HALLWAY;
		}
		else
		{
			playerPosition = ROOM_4;
		}
	}
	else if (position.x > 4 && position.x < 8)
	{
		if (position.z < 0 && position.z > -5)
		{
			playerPosition = ROOM_2;
		}
		else
		{
			playerPosition = ROOM_3;
		}
	}
	else if (position.x < 0 && position.x > -4)
	{
		if (position.z < 0 && position.z > -5)
		{
			playerPosition = ROOM_6;
		}
		else 
		{
			playerPosition = ROOM_5;
		}
	}
	else
	{
		playerPosition = OUT_OF_BOUNDS;
	}
}

GameStateManager::RoomPosition GameStateManager::getPlayerPosition()
{
	return playerPosition;
}