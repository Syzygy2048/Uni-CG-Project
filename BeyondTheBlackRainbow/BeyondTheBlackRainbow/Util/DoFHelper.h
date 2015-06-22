#pragma once
class DoFHelper
{
public:
	~DoFHelper();
	
	static DoFHelper* getInstance();

	void setDistanceToFocusedObject(float distance);
	void update();
	float getFocusDistance();

private:
	DoFHelper();
	DoFHelper(DoFHelper const&);
	void operator=(DoFHelper const&);

	float distanceToFocusedObject = 0;
	float minChangeRate = 0.01;
	float currentFocusDistance = 0;
};

