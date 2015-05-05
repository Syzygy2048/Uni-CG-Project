#pragma once

#include <assimp\scene.h>
#include <GL\glew.h>

#include "SceneNode.h"
#include "../Texture.h"
#include "../InputHandler.h"
#include "../Importers/MeshLoadInfo.h"
#include "../Shader/ShaderProgram.h"
#include "../Physics/PhysicsHandler.h"

class ShaderProgram;

class MeshNode :
	public SceneNode
{
public:
	MeshNode(UUID uuid, aiMesh* triangleMesh, const MeshLoadInfo::LoadInfo* meshLoadInfo);
	~MeshNode();

	//TODO: should probably be moved into Renderer class (renderer->initMeshNodeForRendering(this))
	//MeshNode should only store the data and pass calls through itself.
	void prepareForRendering();
	void unprepareForRendering();
	
	//needs to be called after inserting into the scenegraph because it needs the position which is defined by the parent transform nodes
	void createCollisionShape(PhysicsHandler* physicsHandler);

	virtual void update(double timeStep, InputHandler* input);
	//this takes both the viewprojection matrix as well as the individual matrices so that they don't have to be multiplied per object per frame.
	void draw(glm::mat4 viewMatrix, glm::mat4 projectionMatrix, glm::mat4 viewProjectionMatrix);

	GLuint getVao();
	int getDrawSize();

	GLuint getUV();

	virtual glm::mat4 propagateMatrix();

	glm::mat4 getModelViewProjectionMatrix();
	glm::mat4 getViewProjectionMatrix();
	glm::mat4 getModelMatrix();
	glm::mat4 getViewMatrix();
	glm::mat4 getProjectionMatrix();

	Texture* getTexture(const char* path);

	GLuint getMVPLocation();
	GLuint getTextureLocation();
	ShaderProgram* getShaderProgram();

private:
	aiMesh* triangleMesh;

	GLuint vao;
	GLuint vertexBuffer;
	GLuint indexBuffer;	
	GLuint normalBuffer;
	GLuint uvBuffer;
	GLuint textureBuffer;

	int vertexAttribPointer;
	int normalAttribPointer;
	int textureAttribPointer;

	glm::mat4 modelMatrix;
	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;
	glm::mat4 viewProjectionMatrix;
	
	GLuint myShaderID;
	Texture* myTexture;
	bool textureInit;

	MeshLoadInfo* loadInfo;
	ShaderProgram* shaderProgram;

	physx::PxRigidActor* physicsActor;
};

