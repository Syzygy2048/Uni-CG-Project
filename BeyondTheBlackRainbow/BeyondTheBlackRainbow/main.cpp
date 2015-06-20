#include <stdio.h>
#include <stdlib.h>
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <map>
#include <sstream>

#include "InputHandler.h"
#include "Render\Renderer.h"
#include "GLDebug.h"
#include "Util\UuidHelper.h"
#include "Importers\MeshImporter.h"
#include "SceneGraph\TransformNode.h"
#include "SceneGraph\LightNode\LightsEnum.h"
#include "SceneGraph\CameraNode.h"
#include "SceneGraph\PlayerNode.h"
#include "SceneGraph\LightNode\PointLightNode.h"
#include "SceneGraph\LightNode\DirectionalLightNode.h"
#include "SceneGraph\LightNode\SpotLightNode.h"
#include "Physics\PhysicsHandler.h"
#include "Text\Text.h"
#include "Event\EventFactory.h"
#include "Texture\SamplerStateEnum.h"
#include "Texture\MipmapStateEnum.h"
#include "Framebuffer.h"


std::map < std::string, Text* > text;

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

void initializeText() {
	Text* fpsText = new Text("FPS: 999", MeshLoadInfo::TEXT);
	fpsText->prepareText(50, 100, 25);
	text.insert(std::pair<std::string, Text*>("fpsText", fpsText));
	Text* someText = new Text("Escape the room!", MeshLoadInfo::TEXT);
	someText->prepareText(50, 50, 30);
	someText->setValid(true);
	text.insert(std::pair<std::string, Text*>("helpText", someText));
	Text* nearestSampler = new Text("Sampling: Nearest Neighbour", MeshLoadInfo::TEXT);
	nearestSampler->prepareText(50, 550, 20);
	text.insert(std::pair<std::string, Text*>("nearestSampler", nearestSampler));
	Text* linearSampler = new Text("Sampling: Linear", MeshLoadInfo::TEXT);
	linearSampler->prepareText(50, 550, 20);
	linearSampler->setValid(true);
	text.insert(std::pair<std::string, Text*>("linearSampler", linearSampler));
	Text* nearestMipmap = new Text("Mipmap: Nearest Neighbour", MeshLoadInfo::TEXT);
	nearestMipmap->prepareText(50, 500, 20);
	text.insert(std::pair<std::string, Text*>("nearestMipmap", nearestMipmap));
	Text* bilinearMipmap = new Text("Mipmap: Bilinear", MeshLoadInfo::TEXT);
	bilinearMipmap->prepareText(50, 500, 20);
	text.insert(std::pair<std::string, Text*>("bilinearMipmap", bilinearMipmap));
	Text* trilinearMipmap = new Text("Mipmap: Trilinear", MeshLoadInfo::TEXT);
	trilinearMipmap->prepareText(50, 500, 20);
	trilinearMipmap->setValid(true);
	text.insert(std::pair<std::string, Text*>("trilinearMipmap", trilinearMipmap));
	Text* blendOn = new Text("Transparency On", MeshLoadInfo::TEXT);
	blendOn->prepareText(50, 450, 20);
	blendOn->setValid(true);
	text.insert(std::pair<std::string, Text*>("blendOn", blendOn));
	Text* blendOff = new Text("Transparency Off", MeshLoadInfo::TEXT);
	blendOff->prepareText(50, 450, 20);
	text.insert(std::pair<std::string, Text*>("blendOff", blendOff));
}

void setSamplerText(SamplerState state)
{
	switch (state)
	{
	case NEAREST_N :
		text.find("nearestSampler")->second->setValid(true);
		break;
	case LINEAR:
		text.find("linearSampler")->second->setValid(true);
		break;
	}
}

void blendSamplerText() 
{
	text.find("nearestSampler")->second->setValid(false);
	text.find("linearSampler")->second->setValid(false);
}

void setMipmapText(MipmapState state)
{
	switch (state)
	{
	case NEAREST :
		text.find("nearestMipmap")->second->setValid(true);
		break;
	case BILINEAR:
		text.find("bilinearMipmap")->second->setValid(true);
		break;
	case TRILINEAR:
		text.find("trilinearMipmap")->second->setValid(true);
		break;
	}
}

void blendMipmapText()
{
	text.find("nearestMipmap")->second->setValid(false);
	text.find("bilinearMipmap")->second->setValid(false);
	text.find("trilinearMipmap")->second->setValid(false);
}

void setTransparencyText(bool blend)
{
	switch (blend)
	{
	case true :
		text.find("blendOn")->second->setValid(true);
		break;
	case false :
		text.find("blendOff")->second->setValid(true);
	}
}

void blendTransparencyText()
{
	text.find("blendOn")->second->setValid(false);
	text.find("blendOff")->second->setValid(false);

}

int main() {

	Renderer* renderer = Renderer::getInstance();
	if (renderer->init() == -1){
		return -1;
	}
	
	GLDebug::registerDebugCallbacks();
	
	InputHandler* input = new InputHandler();

	initializeText();

	PhysicsHandler* physics = new PhysicsHandler();
	physics->initPhysics();	

	std::map<std::string, CameraNode*> cameraList;

	//start of part that should be in a scene loader
	CameraNode* activeCamera = new CameraNode(generateUuid());
	
	
	//this way we have a list of cameras and can switch between them as we want just by doing activeCamera = cameraList.find("whichever camera we want")->second;
	cameraList.insert(std::pair<std::string, CameraNode*>(std::string("player camera"), activeCamera));
	
	std::vector<LightNode*> lights;
	LightNode* firstLight = new PointLightNode(generateUuid(), glm::vec3(2.0, 1.5, -4.5), 1.0f, glm::vec3(1, 1, 1), LightType::POINT_LIGHT);
	//LightNode* secondLight = new PointLightNode(generateUuid(), glm::vec3(2.0, 1.0, -1), 2.0f, glm::vec3(1, 1, 1), LightType::POINT_LIGHT);
	LightNode* secondLight = new SpotLightNode(generateUuid(), glm::vec3(2.0, 1.0, -1), 1.0f, glm::vec3(1, 0, 1), glm::vec3(0, -1, 0), glm::vec2(0.5, 0.8), LightType::SPOT_LIGHT);
	//LightNode* thirdLight = new DirectionalLightNode(generateUuid(), glm::vec3(2.0, 1.0, -1), 1.0f, glm::vec3(1, 1, 1), glm::vec3(0, -1, 0), LightType::DIRECTIONAL_LIGHT);
	lights.push_back(firstLight);
	lights.push_back(secondLight);
	//lights.push_back(thirdLight);
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
	transformNodeRoom->attachChild(secondLight);
	//transformNodeRoom->attachChild(thirdLight);
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
	std::map<std::string, Framebuffer*> framebuffers;
	int framebufferWidth = 2048;
	int framebufferHeight = 2048;
	float framebufferNear = 0.1f;
	float framebufferFar = 100.0f;
	Framebuffer* frameBufferPointFirst = new Framebuffer(MeshLoadInfo::DEPTH, framebufferWidth, framebufferHeight);
	//Framebuffer* frameBufferPointSecond = new Framebuffer(MeshLoadInfo::DEPTH, framebufferWidth, framebufferHeight);
	//Framebuffer* frameBufferDir = new Framebuffer(MeshLoadInfo::DEPTHDIR, framebufferWidth, framebufferHeight);
	frameBufferPointFirst->prepareFrameBuffer(firstLight);
	//frameBufferPointSecond->prepareFrameBuffer(secondLight);
	//frameBufferDir->prepareFrameBuffer(DIRECTIONAL_LIGHT);
	frameBufferPointFirst->setNearPlane(framebufferNear);
	//->setNearPlane(framebufferNear);
	frameBufferPointFirst->setFarPlane(framebufferFar);
	//frameBufferPointSecond->setFarPlane(framebufferFar);

	framebuffers.insert(std::pair<std::string, Framebuffer*>("pointLight0", frameBufferPointFirst));
	//framebuffers.insert(std::pair<std::string, Framebuffer*>("pointLight1", frameBufferPointSecond));
	//framebuffers.insert(std::pair<std::string, Framebuffer*>("dirLight", frameBufferDir));
	
	//pointlight
	GLfloat aspect = (GLfloat)framebufferWidth / (GLfloat)framebufferHeight;
	glm::mat4 depthProjectionMatrixPOINT = glm::perspective(90.0f, aspect, framebufferNear, framebufferFar);
	glm::mat4 depthModelMatrix = glm::mat4(1.0);
	std::vector<glm::mat4> depthTransforms;

	//directional light
	glm::mat4 depthProjectionMatrixDIR = glm::ortho<float>(-10, 10, -10, 10, -10, 20);
	//for renderer, directional light is constant
	renderer->setDepthProjectionMatrix(depthProjectionMatrixDIR);
	renderer->setDepthModelMatrix(depthModelMatrix);
	renderer->setFrameBuffers(framebuffers);



	physics->createPhysicsFloor();
	
	double time = glfwGetTime();
	double oldTime = glfwGetTime();
	double timeStep = 1.0 / 60.0;
	double mipmapTime = 0.1;
	double samplerTime = 0.1;
	double blendTime = 0.1;
	int frame = 0;
	double fpsTime = glfwGetTime();
	double fpsTimebase = 0;
	int fps = 0;
	
	bool helpEnable = true;
	bool blendEnable = true;
	bool wireframeEnabled = false;
	bool fpsEnabled = false;
	bool oldF1State = false;
	bool oldF3State = false;
	bool oldF2State = false;
	bool oldF4State = false;
	bool oldF5State = false;
	bool oldF8State = false;
	bool oldF9State = false;

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
		
		//setTextTime oldtime+5
		if (samplerTime != 0.0 && (oldTime - samplerTime) > 5)
		{
			blendSamplerText();
			samplerTime = 0.0;
		}
		if (mipmapTime != 0.0 && (oldTime - mipmapTime) > 5)
		{
			blendMipmapText();
			mipmapTime = 0.0;
		}
		if (blendTime != 0.0 && (oldTime - blendTime) > 5)
		{
			blendTransparencyText();
			blendTime = 0.0;
		}

		input->update(renderer->getWindow());
		
		//help
		if (input->f1 && !oldF1State)
		{
			helpEnable = !helpEnable;
			text.find("helpText")->second->setValid(helpEnable);
		}

		//FPS
		if (input->f2 && !oldF2State)
			fpsEnabled = !fpsEnabled;
		if (fpsEnabled)
		{
			char str[10];
			char fpsString[20] = "FPS: ";

			frame++;
			fpsTime = glfwGetTime();
			if (fpsTime - fpsTimebase > 1) {
				fps = frame / (fpsTime - fpsTimebase);
				fpsTimebase = fpsTime;
				frame = 0;
			}

			//int fps = (1 / deltaTime);
			_itoa_s(fps, str, 10);

			strcat_s(fpsString, 20, str);
			text.find("fpsText")->second->setValid(true);
			text.find("fpsText")->second->setText(fpsString);
		}
		else
		{
			text.find("fpsText")->second->setValid(false);
		}
		
		//wireframe
		if (input->f3 && !oldF3State)
			wireframeEnabled = !wireframeEnabled;
		//wir wollten alle glMethoden in Renderer haben...
		if (wireframeEnabled)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}

		//transparency
		if (input->f9 && !oldF9State)
		{
			blendTransparencyText();
			blendEnable = !blendEnable;
			setTransparencyText(blendEnable);
			blendTime = oldTime;
		}

		glm::mat4 projectionMatrix = activeCamera->getProjectionMatrix();
		glm::mat4 viewMatrix = activeCamera->getViewMatrix();
		glm::mat4 viewProjectionMatrix = projectionMatrix * viewMatrix;
		glm::vec3 playerPosition = glm::vec3(glm::inverse(viewMatrix)[0][3], glm::inverse(viewMatrix)[1][3], glm::inverse(viewMatrix)[2][3]);
		
		std::stringstream keyPointLight;
		//draw shadows
		for (int i = 0; i < lights.size(); i++) {
			if (lights.at(i)->getLightType() == DIRECTIONAL_LIGHT) {
				glm::vec3 lightInvDir = (lights.at(i)->getDirection());
				glm::mat4 depthViewMatrix = glm::lookAt(lightInvDir + playerPosition, glm::vec3(0, 0, 0) + playerPosition, glm::vec3(0, 1, 0));
				framebuffers.find("dirLight")->second->setDepthMVP(depthProjectionMatrixDIR*depthViewMatrix*depthModelMatrix);
				//shadow of meshes
				renderer->bindFrameBuffer(GL_FRAMEBUFFER, framebuffers.find("dirLight")->second->getFramebufferID());
				for (MeshNode* node : drawArray) {
					renderer->drawShadow(node, framebuffers.find("dirLight")->second);
				}
				renderer->unbindFrameBuffer(GL_FRAMEBUFFER);
			}
			else if (lights.at(i)->getLightType() == POINT_LIGHT) {
				glm::vec3 lightPos = lights.at(i)->getPosition();
				depthTransforms.clear();
				depthTransforms.push_back(depthProjectionMatrixPOINT * glm::lookAt(lightPos, lightPos + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
				depthTransforms.push_back(depthProjectionMatrixPOINT * glm::lookAt(lightPos, lightPos + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
				depthTransforms.push_back(depthProjectionMatrixPOINT * glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
				depthTransforms.push_back(depthProjectionMatrixPOINT * glm::lookAt(lightPos, lightPos + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)));
				depthTransforms.push_back(depthProjectionMatrixPOINT * glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)));
				depthTransforms.push_back(depthProjectionMatrixPOINT * glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)));
				keyPointLight << "pointLight";
				keyPointLight << i;
				//std::cout << keyPointLight.str() << std::endl;
				framebuffers.find(keyPointLight.str())->second->setDepthTransforms(depthTransforms);
				//shadow of meshes
				renderer->bindFrameBuffer(GL_FRAMEBUFFER, framebuffers.find(keyPointLight.str())->second->getFramebufferID());
				for (MeshNode* node : drawArray) {
					renderer->drawShadow(node, framebuffers.find(keyPointLight.str())->second);
				}
				renderer->unbindFrameBuffer(GL_FRAMEBUFFER);
				keyPointLight.str(std::string());
				keyPointLight.clear();
			}
		}		
		//draw meshes
		for (MeshNode* node : drawArray){
			if (input->f4 && !oldF4State)
			{
				blendSamplerText();
				node->getTexture()->setSamplerState();
				setSamplerText(node->getTexture()->getSamplerState());
				samplerTime = oldTime;
			}
			//setMipmap
			if (input->f5 && !oldF5State)
			{
				blendMipmapText();
				node->getTexture()->setMipmapState();
				setMipmapText(node->getTexture()->getMipmapState());
				mipmapTime = oldTime;
			}
			node->draw(viewMatrix, projectionMatrix, viewProjectionMatrix, player->getPosition(), framebuffers);
		}
		for (auto const &it : text) {
			if (it.second->getValid()) {
				it.second->draw(blendEnable);
			}
		}

		oldF1State = input->f1;
		oldF2State = input->f2;
		oldF3State = input->f3;
		oldF4State = input->f4;
		oldF5State = input->f5;
		oldF8State = input->f8;
		oldF9State = input->f9;

		glLoadMatrixf(&viewProjectionMatrix[0][0]);

		physics->renderCollisionShapes();
		
		
		glfwSwapBuffers(renderer->getWindow());
		glfwPollEvents();
	}
	
	glfwTerminate();

	return 0;
}