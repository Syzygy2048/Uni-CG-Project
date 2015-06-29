#pragma once

#include <assimp\scene.h>
#include <GL\glew.h>
#include <map>

#include "SceneNode.h"
#include "../Texture/Texture.h"
#include "../InputHandler.h"
#include "../Importers/MeshLoadInfo.h"
#include "../Shader/ShaderProgram.h"
#include "../Physics/PhysicsHandler.h"
#include "../Framebuffer.h"

class ShaderProgram;
class Framebuffer;

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
	void draw(glm::mat4 viewMatrix, glm::mat4 projectionMatrix, glm::mat4 viewProjectionMatrix, glm::vec3 playerPosition, std::map<std::string, Framebuffer*> framebuffers);

	GLuint getVao();
	int getDrawSize();

	GLuint getUV();

	virtual glm::highp_mat4 propagateMatrix();

	glm::mat4 getModelViewProjectionMatrix();
	glm::mat4 getViewProjectionMatrix();
	glm::mat4 getModelMatrix();
	glm::mat4 getViewMatrix();
	glm::mat4 getProjectionMatrix();
	void setDepthBiasMVP(glm::mat4 depthMVP);
	glm::mat4 getDepthBiasMatrix();

	Texture* getTexture();
	void setActiveTexture(Texture* texture); //if nullptr->myTeyture
	void setShadowMap(Texture* shadowMap);
	Texture* getShadowMap();
	glm::vec3 getPlayerPosition();

	GLuint getMVPLocation();
	GLuint getTextureLocation();
	ShaderProgram* getShaderProgram();

	const MeshLoadInfo::LoadInfo* getLoadInfo();

	SceneNode* getParent();

	void removeCollisionShape();

	std::map<std::string, Framebuffer*> getFramebuffers();

	static bool HAVE_KEY;

	void objectFound();
	int getFoundObject();
	void vaseFound();
	int getFoundVase();
	
	void setLightSet(int i);
	int getLightSet();
	bool LIGHT_FOUND = false;

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

	glm::highp_mat4 modelMatrix;
	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;
	glm::mat4 viewProjectionMatrix;
	
	GLuint myShaderID;
	Texture* activeTexture;
	Texture* myTexture;
	Texture* shadowMap;
	bool textureInit;

	const MeshLoadInfo::LoadInfo* loadInfo;
	ShaderProgram* shaderProgram;

	physx::PxRigidActor* physicsActor;

	glm::vec3 playerPosition;

	glm::mat4 biasMatrix = glm::mat4(
		0.5, 0.0, 0.0, 0.0,
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
		0.5, 0.5, 0.5, 1.0
		);
	glm::mat4 depthBiasMatrix;

	std::map<std::string, Framebuffer*> framebuffers;

	int lightSet;
	static int FOUND_OBJECTS;
	bool found = false;
	static int VASE_FOUND;
	bool foundVase = false;
};

