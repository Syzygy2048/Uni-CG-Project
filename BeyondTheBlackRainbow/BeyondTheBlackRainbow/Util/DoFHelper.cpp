#include <cmath>
#include "DoFHelper.h"

#include <iostream>

DoFHelper::DoFHelper()
{
}


DoFHelper::~DoFHelper()
{
}

DoFHelper* DoFHelper::getInstance(){
	static DoFHelper instance;
	return &instance;
}

void DoFHelper::setDistanceToFocusedObject(float distance){
	distanceToFocusedObject = distance;


}

void DoFHelper::update()
{
	float changeRate = 1.f/10 * (distanceToFocusedObject - currentFocusDistance);
	if (abs(currentFocusDistance - distanceToFocusedObject) > minChangeRate)
	{
			currentFocusDistance += changeRate;
	}
	else
	{
		currentFocusDistance = distanceToFocusedObject;
	}
}

float DoFHelper::getFocusDistance(){
	return currentFocusDistance;
}