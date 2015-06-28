#include "Behavior.h"

class RotateBehavior : public Behavior
{
public:
	RotateBehavior();
	~RotateBehavior();

	virtual void update(SceneNode* node, InputHandler* input, double deltaTime);

private:
	float rotationSpeed = 0.1;
};