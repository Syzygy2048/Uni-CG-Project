#pragma once

#include <assimp\scene.h>
#include <GL\glew.h>
#include "SceneNode.h"
#include "..\Texture.h"
#include "..\InputHandler.h"


class MeshNode :
	public SceneNode
{
public:
	MeshNode(UUID uuid, aiMesh* triangleMesh);
	~MeshNode();

	//TODO: should probably be moved into Renderer class (renderer->initMeshNodeForRendering(this))
	//MeshNode should only store the data and pass calls through itself.
	void prepareForRendering();
	void unprepareForRendering();

	void draw(InputHandler* input);

	GLuint getVao();
	int getDrawSize();

	GLuint getUV();

	virtual glm::mat4 propagateMatrix();

	GLuint getShaderID();
	Texture* getTexture(const char* path);
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


	
	GLuint shaderID;
	Texture* texture;
	bool textureInit;
};

