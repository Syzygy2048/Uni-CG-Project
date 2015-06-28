#include "Behavior.h"

class OpenDoorBehavior : public Behavior
{
public:
	OpenDoorBehavior();
	~OpenDoorBehavior();

	virtual void update(SceneNode* node, InputHandler* input, double deltaTime);

private:	
	glm::highp_float rotateBy = 90;
	glm::highp_float rotationAmount = 1;
};