#include "MeshNode.h"
#include "../Render/Renderer.h"


MeshNode::MeshNode(UUID uuid, aiMesh* triangleMesh) : SceneNode(uuid, NodeType::MESH_NODE)
{
	this->triangleMesh = triangleMesh;
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
	//generate Vertex Array for mesh
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
	//create array with faces
	unsigned int* indexArray = (unsigned int*)malloc(sizeof(unsigned int)* triangleMesh->mNumFaces * 3);
	unsigned int index = 0;

	for (unsigned int t = 0; t < triangleMesh->mNumFaces; ++t)
	{
		const struct aiFace* face = &triangleMesh->mFaces[t];

		memcpy(&indexArray[index], face->mIndices, 3 * sizeof(float));
		index += 3;
	}
	
	renderer->fillBuffer(indexBuffer, GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)* triangleMesh->mNumFaces * 3, indexArray, GL_STATIC_DRAW);

	//buffer for vertex positions
	if (triangleMesh->HasPositions()){
		renderer->fillBuffer(vertexBuffer, GL_ARRAY_BUFFER, sizeof(float)* 3 * triangleMesh->mNumVertices, triangleMesh->mVertices, GL_STATIC_DRAW);
		renderer->setVertexAttribPointer(vertexAttribPointer, 3, GL_FLOAT, GL_FALSE, 0, 0);
	}

	//buffer for vertex normals
	if (triangleMesh->HasNormals()){
		//TODO: move the glGetAttribLocation calls into shader class
		//GLint normalIndex = glGetAttribLocation(shader->programHandle, "normal");
		renderer->fillBuffer(normalBuffer, GL_ARRAY_BUFFER, sizeof(float)* 3 * triangleMesh->mNumVertices, triangleMesh->mNormals, GL_STATIC_DRAW);
		renderer->setVertexAttribPointer(normalAttribPointer, 3, GL_FLOAT, GL_FALSE, 0, 0);
	}

	//buffer for texture coordinats
	if (triangleMesh->HasTextureCoords(0)){
		float *texCoords = (float*)malloc(sizeof(float)* 2 * triangleMesh->mNumVertices);
		for (unsigned int k = 0; k < triangleMesh->mNumVertices; ++k){
			texCoords[k * 2] = triangleMesh->mTextureCoords[0][k].x;
			texCoords[k * 2 + 1] = triangleMesh->mTextureCoords[0][k].y;
		}

		//GLint uvIndex = glGetAttribLocation(shader->programHandle, "uv");
		renderer->fillBuffer(textureBuffer, GL_ARRAY_BUFFER, sizeof(float)* 2 * triangleMesh->mNumVertices, texCoords, GL_STATIC_DRAW);
		renderer->setVertexAttribPointer(textureAttribPointer, 2, GL_FLOAT, GL_FALSE, 0, 0);
	}


	renderer->bindVertexArray(0);
	renderer->bindBuffer(GL_ARRAY_BUFFER, 0);
	renderer->bindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	delete indexArray;
}

void MeshNode::draw()
{
	Renderer::getInstance()->draw(this);


	//Renderer::UseShader();
	//Renderer::bindThisMesh();

	//Push data to shader



}

GLuint MeshNode::getVao()
{
	return vao;
}

int MeshNode::getDrawSize()
{
	return triangleMesh->mNumFaces * 3;
}