#include "MeshNode.h"

#include <iostream>
#include <glm\gtc\quaternion.hpp>

#include "../Render/Renderer.h"
#include "../Importers/MeshImporter.h"
#include "../Importers/ShaderImporter.h"
#include "TransformNode.h"

int MeshNode::FOUND_OBJECTS = 0;
int MeshNode::VASE_FOUND = 0;
bool MeshNode::HAVE_KEY = false;

MeshNode::MeshNode(int uuid, aiMesh* triangleMesh, const MeshLoadInfo::LoadInfo* meshLoadInfo) : SceneNode(uuid, NodeType::MESH_NODE)
{
	this->triangleMesh = triangleMesh;
	loadInfo = meshLoadInfo;
	textureInit = false;
}


MeshNode::~MeshNode()
{
	glDeleteBuffers(1, &vertexBuffer);
	glDeleteBuffers(1, &indexBuffer);
	//glDeleteProgram(programID);
	glDeleteVertexArrays(1, &vao);
}

glm::highp_mat4 MeshNode::propagateMatrix()
{
	return parent->propagateMatrix();
}

void MeshNode::prepareForRendering()
{
	Renderer* renderer = Renderer::getInstance();
	renderer->generateVertexArray(&vao);
	renderer->generateBufferObject(&vertexBuffer);
	renderer->generateBufferObject(&indexBuffer);
	renderer->generateBufferObject(&normalBuffer);
	renderer->generateBufferObject(&uvBuffer);
	renderer->generateBufferObject(&textureBuffer);

	vertexAttribPointer = 0;
	normalAttribPointer = 1;
	textureAttribPointer = 2;

	renderer->bindVertexArray(vao);
	unsigned int* indexArray = (unsigned int*)malloc(sizeof(unsigned int)* triangleMesh->mNumFaces * 3);
	unsigned int index = 0;

	for (unsigned int t = 0; t < triangleMesh->mNumFaces; ++t)
	{
		const struct aiFace* face = &triangleMesh->mFaces[t];
		memcpy(&indexArray[index], face->mIndices, 3 * sizeof(float));
		index += 3;
	}
	renderer->fillBuffer(indexBuffer, GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)* triangleMesh->mNumFaces * 3, indexArray, GL_STATIC_DRAW);

	if (triangleMesh->HasPositions()){
		renderer->fillBuffer(vertexBuffer, GL_ARRAY_BUFFER, sizeof(float) * 3 * triangleMesh->mNumVertices, triangleMesh->mVertices, GL_STATIC_DRAW);
		renderer->setVertexAttribPointer(vertexAttribPointer, 3, GL_FLOAT, GL_FALSE, 0, 0);
	}

	if (triangleMesh->HasNormals()){
		renderer->fillBuffer(normalBuffer, GL_ARRAY_BUFFER, sizeof(float) * 3 * triangleMesh->mNumVertices, triangleMesh->mNormals, GL_STATIC_DRAW);
		renderer->setVertexAttribPointer(normalAttribPointer, 3, GL_FLOAT, GL_FALSE, 0, 0);
	}

	float* texCoords = texCoords = (float*)malloc(sizeof(float)* 2 * triangleMesh->mNumVertices);
	if (triangleMesh->HasTextureCoords(0)){
		for (unsigned int k = 0; k < triangleMesh->mNumVertices; ++k){
			texCoords[k * 2] = triangleMesh->mTextureCoords[0][k].x;
			texCoords[k * 2 + 1] = triangleMesh->mTextureCoords[0][k].y;
		}
		renderer->fillBuffer(textureBuffer, GL_ARRAY_BUFFER, sizeof(float) * 2 * triangleMesh->mNumVertices, texCoords, GL_STATIC_DRAW);
		renderer->setVertexAttribPointer(textureAttribPointer, 2, GL_FLOAT, GL_FALSE, 0, 0);
	}

	/**/
	shaderProgram = ShaderImporter::getInstance()->loadShaderProgram(loadInfo->shaderInfo);
	shaderProgram->loadUniformLocations();
	myTexture = new Texture((loadInfo->texturePath).c_str());
	activeTexture = myTexture;
	/**/

	renderer->bindVertexArray(0);
	renderer->bindBuffer(GL_ARRAY_BUFFER, 0);
	renderer->bindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	delete indexArray;
	delete texCoords;
}

void MeshNode::createCollisionShape(PhysicsHandler* physicsHandler)
{
	glm::highp_mat4 modelMatrix = propagateMatrix();
	physicsActor = physicsHandler->createRigidActor(glm::mat4(propagateMatrix()), triangleMesh, loadInfo);
	physicsActor->userData = getUuid();
	physicsHandler->addActorToScene(physicsActor);
}

void MeshNode::removeCollisionShape()
{
	if (physicsActor){
		physicsActor->release();
		physicsActor = nullptr;
	}
}

void MeshNode::update(double timeStep, InputHandler* input)
{
	//TODO update render->physics, simulate physics, update physics->render
	if (behavior != nullptr)
	{
		behavior->update(this, input, timeStep);
	}
	if (parent->getType() == NodeType::TRANSFORM_NODE)
	{		
		if (!physicsActor) return;

		physx::PxTransform trans = physicsActor->getGlobalPose();
		physx::PxVec3 pos = trans.p;
		//physx::PxVec3 rotAxis(0, 0, 0);
		//physx::PxReal rotAngle(0);
		//trans.q.toRadiansAndUnitAxis(rotAngle, rotAxis);
		//oldPosition = position;
		//glm::vec3 position(pos.x, pos.y, pos.z);

		//glm::vec3 rotationAxis(rotAxis.x, rotAxis.y, rotAxis.z);
		//glm::quat newQuat(trans.q.w, trans.q.x, trans.q.y, trans.q.z); // glm::angleAxis(rotAngle, rotationAxis);

		TransformNode* node = (TransformNode *)parent;
		
		glm::highp_mat4 modelMatrix = propagateMatrix();
		glm::highp_mat4 transMatrix = node->getTransform();

		glm::highp_mat4 modelWithoutTrans = glm::inverse(transMatrix) * modelMatrix;

		physx::PxMat44 physicsMatrix(trans);
		glm::highp_mat4 glmMatrix = glm::highp_mat4(
			physicsMatrix.column0.x, physicsMatrix.column0.y, physicsMatrix.column0.z, physicsMatrix.column0.w,
			physicsMatrix.column1.x, physicsMatrix.column1.y, physicsMatrix.column1.z, physicsMatrix.column1.w,
			physicsMatrix.column2.x, physicsMatrix.column2.y, physicsMatrix.column2.z, physicsMatrix.column2.w,
			physicsMatrix.column3.x, physicsMatrix.column3.y, physicsMatrix.column3.z, physicsMatrix.column3.w);
		glm::highp_mat4 difference = glm::inverse(modelWithoutTrans) * glmMatrix;

		physx::PxMat44 noRotation = trans.getInverse() * trans;

		//transMatrix.
		transMatrix = transMatrix * difference;

		
		//glm::quat oldQuat = glm::quat_cast(modelMatrix); 

		//glm::quat deltaQuat = newQuat * glm::inverse(oldQuat);
		
		//position = position - glm::vec3(modelMatrix[3][0], modelMatrix[3][1], modelMatrix[3][2]);	//position difference between old and new transform after physics simulation
		
		
		//glm::vec3 transToZero(transMatrix[3]);
		
		//transMatrix = glm::mat4_cast(deltaQuat) * transMatrix;
		//transMatrix = glm::translate(transMatrix, glm::highp_vec3(position));
		//transMatrix = glm::translate(transMatrix, -transToZero);
		
		//transMatrix = glm::rotate(glm::highp_mat4(transMatrix), glm::angle(deltaQuat), glm::highp_vec3(0,1,0));;
		//transMatrix = glm::translate(transMatrix, transToZero);
		//glm::scatransMatrix->
		
		node->setNewTransform(difference);
	}
	else
	{
		std::cerr << "Node is not attached to a TransformNode, that shouldn't happen." << std::endl;
	}
}

void MeshNode::draw(glm::mat4 viewMatrix, glm::mat4 projectionMatrix, glm::mat4 viewProjectionMatrix, glm::vec3 playerPosition, std::map<std::string, Framebuffer*> framebuffers)
{
	this->viewMatrix = viewMatrix;
	this->projectionMatrix = projectionMatrix;
	this->viewProjectionMatrix = viewProjectionMatrix;
	this->playerPosition = playerPosition;
	this->framebuffers = framebuffers;
	Renderer::getInstance()->draw(this);
}

GLuint MeshNode::getVao()
{
	return vao;
}

GLuint MeshNode::getUV()
{
	return uvBuffer;
}

int MeshNode::getDrawSize()
{
	return triangleMesh->mNumFaces * 3;
}

Texture* MeshNode::getTexture()
{
	return  activeTexture;
}

glm::mat4 MeshNode::getModelViewProjectionMatrix()
{
	return projectionMatrix * viewMatrix * glm::mat4(propagateMatrix());
}
glm::mat4 MeshNode::getViewMatrix()
{
	return viewMatrix;
}

glm::mat4 MeshNode::getProjectionMatrix()
{
	return projectionMatrix;
}

glm::mat4 MeshNode::getViewProjectionMatrix()
{
	return viewProjectionMatrix;
}

glm::mat4 MeshNode::getModelMatrix()
{
	return glm::mat4(propagateMatrix());
}

ShaderProgram* MeshNode::getShaderProgram()
{
	return shaderProgram;
}

const MeshLoadInfo::LoadInfo* MeshNode::getLoadInfo()
{
	return loadInfo;
}

void MeshNode::setDepthBiasMVP(glm::mat4 depthMVP)
{
	depthBiasMatrix = biasMatrix * depthMVP;
}

glm::mat4 MeshNode::getDepthBiasMatrix()
{
	return depthBiasMatrix;
}

void MeshNode::setShadowMap(Texture* shadowMap)
{
	this->shadowMap = shadowMap;
}

Texture* MeshNode::getShadowMap()
{
	return shadowMap;
}

glm::vec3 MeshNode::getPlayerPosition()
{
	return playerPosition;
}

std::map<std::string, Framebuffer*> MeshNode::getFramebuffers()
{
	return framebuffers;
}

void MeshNode::setLightSet(int i)
{
	lightSet = i;
}

int MeshNode::getLightSet()
{
	return lightSet;
}

void MeshNode::objectFound()
{
	if (!found)
	{
		FOUND_OBJECTS++;
		found = true;
	}
}

int MeshNode::getFoundObject()
{
	return FOUND_OBJECTS;
}

void MeshNode::vaseFound()
{
	if (!foundVase)
	{
		VASE_FOUND++;
		foundVase = true;
	}
}
int MeshNode::getFoundVase()
{
	return VASE_FOUND;
}

void MeshNode::setActiveTexture(Texture* texture)
{
	if (texture == nullptr)
	{
		activeTexture = myTexture;
	}
	else {
		activeTexture = texture;
	}
}
