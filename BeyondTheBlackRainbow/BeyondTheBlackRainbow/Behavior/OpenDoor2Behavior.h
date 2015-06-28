#include "Behavior.h"

class OpenDoor2Behavior : public Behavior
{
public:
	OpenDoor2Behavior();
	~OpenDoor2Behavior();

	virtual void update(SceneNode* node, InputHandler* input, double deltaTime);

private:	
	glm::highp_float rotateBy = 90;
	glm::highp_float rotationAmount = 1;
};