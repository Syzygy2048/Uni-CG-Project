#include "../InputHandler.h"
#include "../Text/Text.h"

bool wireframeEnabled = false;
bool fpsEnabled = false;
bool oldF3State = false;
bool oldF2State = false;

std::vector<Text*> handleFButtons(InputHandler* input, double deltaTime, Text* fpsText, std::vector<Text*>* textArray){
	if (input->f2 && !oldF2State)
		fpsEnabled = !fpsEnabled;
	oldF2State = input->f2;
	if (fpsEnabled)
	{
		char str[10];
		char fpsString[20] = "FPS: ";

		int fps = (1000 / deltaTime);
		_itoa_s(fps, str, 10);

		strcat_s(fpsString, 20, str);
		fpsText->setText(fpsString);

		std::vector<Text*> tempTextArray = *textArray;
		for (std::vector<Text*>::iterator iter = textArray->begin(); iter != textArray->end(); iter++)
		{
			bool alreadyAdded = false;
			if (*iter == fpsText)
			{
				alreadyAdded = true;
			}
			if (!alreadyAdded)
			{
				tempTextArray.push_back(fpsText);
			}
		}
		textArray = &tempTextArray;
	}
	else
	{
		std::vector<Text*> tempTextArray = std::vector<Text*>();
		for (std::vector<Text*>::iterator iter = textArray->begin(); iter != textArray->end(); iter++)
		{
			if (*iter != fpsText)
			{
				tempTextArray.push_back(*iter);
			}
		}
		textArray = &tempTextArray;
	}


	if (input->f3 && !oldF3State)
		wireframeEnabled = !wireframeEnabled;
	oldF3State = input->f3;

	if (wireframeEnabled)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	return *textArray;
}