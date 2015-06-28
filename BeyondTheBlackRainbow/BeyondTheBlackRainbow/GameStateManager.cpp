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