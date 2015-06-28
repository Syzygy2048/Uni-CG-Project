#pragma once
class GameStateManager
{
public:
	~GameStateManager();

	static GameStateManager* getInstance();
	
	void setPostProcessingEnabled(bool postProcessingEnabled);
	bool getPostProcessingEnabled();

private:
	GameStateManager();
	GameStateManager(GameStateManager const&);
	void operator=(GameStateManager const&);
	
	bool postProcessingEnabled = false;
};

