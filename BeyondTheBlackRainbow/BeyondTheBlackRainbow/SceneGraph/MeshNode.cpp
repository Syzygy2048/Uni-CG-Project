#include "MeshNode.h"

#include <iostream>

#include "../Render/Renderer.h"
#include "../Importers/MeshImporter.h"
#include "../Importers/ShaderImporter.h"
#include "TransformNode.h"

MeshNode::MeshNode(UUID uuid, aiMesh* triangleMesh, const MeshLoadInfo::LoadInfo* meshLoadInfo) : SceneNode(uuid, NodeType::MESH_NODE)
{
	this->triangleMesh = triangleMesh;
	loadInfo = meshLoadInfo;
	//shaderID = LoadShaders("SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader");
	textureInit = false;
}


MeshNode::~MeshNode()
{
	glDeleteBuffers(1, &vertexBuffer);
	glDeleteBuffers(1, &indexBuffer);
	//glDeleteProgram(programID);
	glDeleteVertexArrays(1, &vao);
}

glm::mat4 MeshNode::propagateMatrix()
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

	if (triangleMesh->HasTextureCoords(0)){
		float *texCoords = (float*)malloc(sizeof(float) * 2 * triangleMesh->mNumVertices);
		for (unsigned int k = 0; k < triangleMesh->mNumVertices; ++k){
			texCoords[k * 2] = triangleMesh->mTextureCoords[0][k].x;
			texCoords[k * 2 + 1] = triangleMesh->mTextureCoords[0][k].y;
		}
		renderer->fillBuffer(textureBuffer, GL_ARRAY_BUFFER, sizeof(float) * 2 * triangleMesh->mNumVertices, texCoords, GL_STATIC_DRAW);
		renderer->setVertexAttribPointer(textureAttribPointer, 2, GL_FLOAT, GL_FALSE, 0, 0);
	}

	/**/
	
	shaderProgram = ShaderImporter::getInstance()->loadShaderProgram(loadInfo->shaderInfo);
	//renderer->linkShader(shaderProgram);
	shaderProgram->loadUniformLocations();
	myTexture = new Texture((loadInfo->texturePath).c_str());
	//myTexture->bind(0);
	/**/

	renderer->bindVertexArray(0);
	renderer->bindBuffer(GL_ARRAY_BUFFER, 0);
	renderer->bindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	delete indexArray;
}

void MeshNode::createCollisionShape(PhysicsHandler* physicsHandler)
{
	physicsActor = physicsHandler->createDynamicActor(propagateMatrix(), physicsHandler->createSphereCollisionShape(), physicsHandler->createPhysicsMaterial(0.01, 0.01, 0.1f), 0.3);
	physicsHandler->addActorToScene(physicsActor);
}

void MeshNode::update(double timeStep, InputHandler* input)
{
	if (!physicsActor) return;
	physx::PxTransform trans = physicsActor->getGlobalPose();
	physx::PxVec3 pos = trans.p;
	physx::PxVec3 rotAxis(0, 0, 0);
	physx::PxReal rotAngle(0);
	trans.q.toRadiansAndUnitAxis(rotAngle, rotAxis);

	glm::vec3 position(pos.x, pos.y, pos.z);
	
	glm::vec3 rotationAxis(rotAxis.x, rotAxis.y, rotAxis.z);

	if (parent->getType() == NodeType::TRANSFORM_NODE)
	{
		TransformNode* node = (TransformNode *)parent;
		//glm::mat4 trans = node->getTransform();
		glm::mat4 posMat = glm::translate(glm::mat4(1.0f), position);
		glm::rotate(posMat, rotAngle, rotationAxis);
		node->setNewTransform(posMat);
	}
	else
	{
		std::cerr << "Node is not attached to a TransformNode, that shouldn't happen." << std::endl;
	}
}

void MeshNode::draw(glm::mat4 viewMatrix, glm::mat4 projectionMatrix, glm::mat4 viewProjectionMatrix)
{
	this->viewMatrix = viewMatrix;
	this->projectionMatrix = projectionMatrix;
	this->viewProjectionMatrix = viewProjectionMatrix;
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
	return  myTexture;
}

glm::mat4 MeshNode::getModelViewProjectionMatrix()
{
	return projectionMatrix * viewMatrix * propagateMatrix();
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
	return propagateMatrix();
}

ShaderProgram* MeshNode::getShaderProgram()
{
	return shaderProgram;
}

const MeshLoadInfo::LoadInfo* MeshNode::getLoadInfo()
{
	return loadInfo;
}