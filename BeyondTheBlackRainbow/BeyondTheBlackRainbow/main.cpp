#include <stdio.h>
#include <stdlib.h>
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <map>

#include "InputHandler.h"
#include "Render\Renderer.h"
#include "GLDebug.h"
#include "Util\UuidHelper.h"
#include "Importers\MeshImporter.h"
#include "SceneGraph\TransformNode.h"
#include "SceneGraph\LightsEnum.h"
#include "SceneGraph\CameraNode.h"
#include "SceneGraph\PlayerNode.h"
#include "SceneGraph\PointLightNode.h"
#include "SceneGraph\DirectionalLightNode.h"
#include "Physics\PhysicsHandler.h"
#include "Text\Text.h"
#include "Event\EventFactory.h"
#include "Framebuffer.h"




void spawn20Ducks(SceneNode* sceneGraph, PhysicsHandler* physicsHandler, std::vector<MeshNode*>* drawArray)
{
	
	for (int i = 0; i < 20; i++){
		float randomX = ((std::rand() % 100) - 50) / 100.f;
		float randomY = ((std::rand() % 100) - 50) / 100.f;
		float randomZ = ((std::rand() % 100) - 50) / 100.f;
		SceneNode* debugTransform = new TransformNode(generateUuid(), glm::mat4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		2 + randomX, 2 + randomY, -3.5 + randomZ, 1));
		MeshNode* debugMesh = MeshImporter::getInstance()->getMesh(MeshLoadInfo::DUCK);
		debugTransform->attachChild(debugMesh);
		sceneGraph->attachChild(debugTransform);
		drawArray->push_back(debugMesh);
		debugMesh->prepareForRendering();
		debugMesh->createCollisionShape(physicsHandler);
	}
}



int main() {

	Renderer* renderer = Renderer::getInstance();
	if (renderer->init() == -1){
		return -1;
	}
	
	GLDebug::registerDebugCallbacks();
	
	InputHandler* input = new InputHandler();

	PhysicsHandler* physics = new PhysicsHandler();
	physics->initPhysics();

	std::vector<Text*> textArray;	//should probably be a key value pair for easier manipulation

	Text* fpsText = new Text("FPS: 999", MeshLoadInfo::TEXT);
	fpsText->prepareText(100, 100, 25);
	Text* someText = new Text("stuff", MeshLoadInfo::TEXT);
	someText->prepareText(50, 50, 50);

	textArray.push_back(fpsText);
	textArray.push_back(someText);
	

	std::map<std::string, CameraNode*> cameraList;

	//start of part that should be in a scene loader
	CameraNode* activeCamera = new CameraNode(generateUuid());
	
	
	//this way we have a list of cameras and can switch between them as we want just by doing activeCamera = cameraList.find("whichever camera we want")->second;
	cameraList.insert(std::pair<std::string, CameraNode*>(std::string("player camera"), activeCamera));
	
	//glm::vec3 lightPos = glm::vec3(2, 1.5, -2.5);
	std::vector<LightNode*> lights;
	LightNode* firstLight = new PointLightNode(generateUuid(), glm::vec3(1, 1.5, -2.5), 1.0f, glm::vec3(1, 1, 1), LightType::POINT_LIGHT);
	//LightNode* secondLight = new PointLightNode(generateUuid(), glm::vec3(2, 1.5, -1), 1.0f, glm::vec3(1, 1, 1), LightType::POINT_LIGHT);
	//LightNode* thirdLight = new DirectionalLightNode(generateUuid(), glm::vec3(0, 0, 0), 1.0f, glm::vec3(1, 1, 1), glm::vec3(1, 1, 1), LightType::DIRECTIONAL_LIGHT);
	//LightNode* fourthLight = new DirectionalLightNode(generateUuid(), glm::vec3(0, 2, 0), 1.0f, glm::vec3(0, 0, 1), glm::vec3(1, 1, 1), LightType::DIRECTIONAL_LIGHT);
	lights.push_back(firstLight);
	//lights.push_back(secondLight);
	//lights.push_back(thirdLight);
	//lights.push_back(fourthLight);
	renderer->setLights(lights);

	MeshNode* tableMesh = MeshImporter::getInstance()->getMesh(MeshLoadInfo::TABLE);
	MeshNode* duckMesh = MeshImporter::getInstance()->getMesh(MeshLoadInfo::DUCK);
	MeshNode* bedMesh = MeshImporter::getInstance()->getMesh(MeshLoadInfo::BED);
	MeshNode* roomMesh = MeshImporter::getInstance()->getMesh(MeshLoadInfo::ROOM);
	MeshNode* doorMesh = MeshImporter::getInstance()->getMesh(MeshLoadInfo::DOOR);
	tableMesh->prepareForRendering();
	duckMesh->prepareForRendering();
	bedMesh->prepareForRendering();
	roomMesh->prepareForRendering();
	doorMesh->prepareForRendering();

	std::vector<MeshNode*> drawArray;
	drawArray.push_back(tableMesh);
	drawArray.push_back(duckMesh);
	drawArray.push_back(bedMesh);
	drawArray.push_back(roomMesh);
	drawArray.push_back(doorMesh);

	SceneNode* sceneGraph = new SceneNode(generateUuid(), NodeType::ROOT_NODE);
	sceneGraph->setParent(nullptr);

	EventManager* eventManager = new EventManager(sceneGraph);
	sceneGraph->setEventManager(eventManager);
	tableMesh->setEventManager(eventManager);
	duckMesh->setEventManager(eventManager);
	bedMesh->setEventManager(eventManager);
	roomMesh->setEventManager(eventManager);
	doorMesh->setEventManager(eventManager);
	
	
	SceneNode* transformNodeRoom = new TransformNode(generateUuid(), glm::mat4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1));
	//glm::rotate(transformNodeRoom->propagateMatrix(), 90.0f , glm::vec3(1, 0, 0));
	SceneNode* transformNodeDuck = new TransformNode(generateUuid(), glm::mat4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		1, -0.1, -3.5, 1));
	SceneNode* transformNodeBed = new TransformNode(generateUuid(), glm::mat4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		3.4, 0, -1.01, 1));
	SceneNode* transformNodeTable = new TransformNode(generateUuid(), glm::mat4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1));
	SceneNode* transformNodeDoor = new TransformNode(generateUuid(), glm::mat4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		2, 0, -5.1, 1));
	transformNodeRoom->attachChild(roomMesh);
	transformNodeRoom->attachChild(transformNodeDuck);
	transformNodeRoom->attachChild(transformNodeBed);
	transformNodeRoom->attachChild(transformNodeTable);
	transformNodeRoom->attachChild(firstLight);
	//transformNodeRoom->attachChild(secondLight);
	//transformNodeRoom->attachChild(thirdLight);
	//transformNodeRoom->attachChild(fourthLight);
	transformNodeDuck->attachChild(duckMesh);
	transformNodeBed->attachChild(bedMesh);
	transformNodeTable->attachChild(tableMesh);
	transformNodeDoor->attachChild(doorMesh);
	
	sceneGraph->attachChild(transformNodeRoom);
	sceneGraph->attachChild(transformNodeDoor);
	
	SceneNode* playerTransform = new TransformNode(generateUuid(), glm::mat4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		2, 0, -2.5, 1));

	PlayerNode* player = new PlayerNode(generateUuid());
	playerTransform->attachChild(activeCamera);
	playerTransform->attachChild(player);
	sceneGraph->attachChild(playerTransform);
	player->setEventManager(eventManager);
	
	roomMesh->createCollisionShape(physics);
	player->createCollisionShape(physics);
	tableMesh->createCollisionShape(physics);
	duckMesh->createCollisionShape(physics);
	bedMesh->createCollisionShape(physics);
	doorMesh->createCollisionShape(physics);

	bedMesh->registerEvent(EventFactory::createEvent(EventTrigger::RAYTRACE_HIT, EventIdentifier::DOOR_TRIGGER));
	doorMesh->registerEvent(EventFactory::createEvent(EventTrigger::EVENT, EventIdentifier::OPEN_DOOR));

	spawn20Ducks(sceneGraph, physics, &drawArray);
	
	
	
	//should probably done recursively in sceneNode		
	
	//end of part that should be in a scene loader


	//Framebuffer stuff (for one pointlight)
	int framebufferWidth = 128;
	int framebufferHeight = 128;
	float framebufferNear = 0.1f;
	float framebufferFar = 100.0f;
	Framebuffer* frameBuffer1 = new Framebuffer(MeshLoadInfo::DEPTH, framebufferWidth, framebufferHeight);
	frameBuffer1->prepareFrameBuffer(POINT_LIGHT);
	frameBuffer1->setNearPlane(framebufferNear);
	frameBuffer1->setFarPlane(framebufferFar);
	//Framebuffer* frameBuffer2 = new Framebuffer(MeshLoadInfo::DEPTH);
	//frameBuffer2->prepareFrameBuffer();
	std::vector<Framebuffer*> framebuffers;
	framebuffers.push_back(frameBuffer1);
	//framebuffers.push_back(frameBuffer2);

	//directional light
	//glm::mat4 depthProjectionMatrix = glm::ortho<float>(-10, 10, -10, 10, -10, 20);
	//glm::mat4 depthModelMatrix = glm::mat4(1.0);

	//pointlight
	GLfloat aspect = (GLfloat)framebufferWidth / (GLfloat)framebufferHeight;
	glm::mat4 depthProjectionMatrix = glm::perspective(90.0f, aspect, framebufferNear, framebufferFar);
	glm::mat4 depthModelMatrix = glm::mat4(1.0);

	std::vector<glm::mat4> depthTransforms;
	renderer->setDepthProjectionMatrix(depthProjectionMatrix);
	renderer->setDepthModelMatrix(depthModelMatrix);
	renderer->setFrameBuffers(framebuffers);



	physics->createPhysicsFloor();
	
	double time = glfwGetTime();
	double oldTime = glfwGetTime();
	double timeStep = 1.0 / 60.0;
	
	bool wireframeEnabled = false;
	bool fpsEnabled = false;
	bool oldF3State = false;
	bool oldF2State = false;

	//gameloop
	double timeOld = 0;
	while (!input->esc && glfwWindowShouldClose(renderer->getWindow()) == 0) {
		// Clear the screen
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		time = glfwGetTime();
		double deltaTime = time - oldTime;
		while (deltaTime > timeStep)
		{
			deltaTime -= timeStep;
			physics->updatePhysics();
			sceneGraph->update(timeStep, input);
		}
		oldTime = time - deltaTime;
		
		input->update(renderer->getWindow());
		

		if (input->f2 && !oldF2State)
			fpsEnabled = !fpsEnabled;
		oldF2State = input->f2;
		if (fpsEnabled)
		{
			char str[10];
			char fpsString[20] = "FPS: ";

			int fps = (1 / deltaTime);
			_itoa_s(fps, str, 10);

			strcat_s(fpsString, 20, str);
			fpsText->setText(fpsString);

			std::vector<Text*> tempTextArray = textArray;
			for (std::vector<Text*>::iterator iter = textArray.begin(); iter != textArray.end(); iter++)
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
			textArray = tempTextArray;
		}
		else
		{
			std::vector<Text*> tempTextArray;
			for (std::vector<Text*>::iterator iter = textArray.begin(); iter != textArray.end(); iter++)
			{				
				if (*iter != fpsText)
				{
					tempTextArray.push_back(*iter);
				}
			}
			textArray = tempTextArray;
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

		glm::mat4 projectionMatrix = activeCamera->getProjectionMatrix();
		glm::mat4 viewMatrix = activeCamera->getViewMatrix();
		glm::mat4 viewProjectionMatrix = projectionMatrix * viewMatrix;
		glm::vec3 playerPosition = glm::vec3(glm::inverse(viewMatrix)[0][3], glm::inverse(viewMatrix)[1][3], glm::inverse(viewMatrix)[2][3]);
		//draw shadows
		for (LightNode* light : lights) {
			int numDirLight = 0;
			int numPointLight = 0;
			if (light->getLightType() == DIRECTIONAL_LIGHT) { // do not work currently
				glm::vec3 lightInvDir = (light->getDirection());
				glm::mat4 depthViewMatrix = glm::lookAt(lightInvDir + playerPosition, glm::vec3(0, 0, 0) + playerPosition, glm::vec3(0, 1, 0));
				framebuffers.at(numDirLight)->setDepthMVP(depthProjectionMatrix*depthViewMatrix*depthModelMatrix);
				//shadow of meshes
				renderer->bindFrameBuffer(GL_FRAMEBUFFER, framebuffers.at(numDirLight)->getFramebufferID());
				for (MeshNode* node : drawArray) {
					renderer->drawShadow(node, framebuffers.at(numDirLight));
				}
				renderer->unbindFrameBuffer(GL_FRAMEBUFFER);
				numDirLight++;
			}
			else if (light->getLightType() == POINT_LIGHT) {
				glm::vec3 lightPos = light->getPosition();
				depthTransforms.push_back(depthProjectionMatrix * glm::lookAt(lightPos, lightPos + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
				depthTransforms.push_back(depthProjectionMatrix * glm::lookAt(lightPos, lightPos + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
				depthTransforms.push_back(depthProjectionMatrix * glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
				depthTransforms.push_back(depthProjectionMatrix * glm::lookAt(lightPos, lightPos + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)));
				depthTransforms.push_back(depthProjectionMatrix * glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)));
				depthTransforms.push_back(depthProjectionMatrix * glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)));
				framebuffers.at(numPointLight)->setDepthTransforms(depthTransforms);
				//shadow of meshes
				renderer->bindFrameBuffer(GL_FRAMEBUFFER, framebuffers.at(numPointLight)->getFramebufferID());
				for (MeshNode* node : drawArray) {
					renderer->drawShadow(node, framebuffers.at(numDirLight));
				}
				renderer->unbindFrameBuffer(GL_FRAMEBUFFER);
				numPointLight++;
			}
		}
		//draw meshes
		for (MeshNode* node : drawArray){
			//std::cout << player->getPosition().x << ", " << player->getPosition().y << ", " << player->getPosition().z << std::endl;
			node->draw(viewMatrix, projectionMatrix, viewProjectionMatrix, player->getPosition());

		}
		for (Text* text : textArray){
			text->draw();
		}

		glLoadMatrixf(&viewProjectionMatrix[0][0]);

		physics->renderCollisionShapes();
		
		
		glfwSwapBuffers(renderer->getWindow());
		glfwPollEvents();
	}
	
	glfwTerminate();

	return 0;
}