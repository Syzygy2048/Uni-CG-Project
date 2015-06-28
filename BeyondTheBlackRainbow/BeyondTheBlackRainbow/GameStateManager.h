#pragma once

#include "glm\glm.hpp"

class GameStateManager
{
public:
	enum RoomPosition{
		ROOM_1,
		ROOM_2,
		ROOM_3,
		ROOM_4,
		ROOM_5,
		ROOM_6,
		HALLWAY,
		OUT_OF_BOUNDS
	};
	
	~GameStateManager();

	static GameStateManager* getInstance();
	
	void setPostProcessingEnabled(bool postProcessingEnabled);
	bool getPostProcessingEnabled();

	void updatePlayerPosition(glm::vec3 position);
	RoomPosition getPlayerPosition();
	
	
private:
	GameStateManager();
	GameStateManager(GameStateManager const&);
	void operator=(GameStateManager const&);
	
	bool postProcessingEnabled = false;

	RoomPosition playerPosition = RoomPosition::ROOM_1;
};

