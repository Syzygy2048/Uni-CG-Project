#pragma once

#include <glm\glm.hpp>


class ViewFrustumHandler
{
public:
	ViewFrustumHandler();
	~ViewFrustumHandler();

	bool checkIfInside(glm::mat4 modelMatrix);
};

