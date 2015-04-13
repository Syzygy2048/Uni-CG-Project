#include "MeshImporter.h"

#include <assimp/postprocess.h>
#include <assimp/cimport.h>
#include <assimp/scene.h>		
#include "../Util/UuidHelper.h"


MeshImporter::MeshImporter()
{
}


MeshImporter::~MeshImporter()
{
}


void MeshImporter::loadMesh(MeshLoadInfo::LoadInfo* loadInfo)
{
	const aiScene* aScene = aiImportFileEx(loadInfo->meshPath.c_str(),
		aiProcessPreset_TargetRealtime_Quality |
		aiProcess_JoinIdenticalVertices |
		aiProcess_FindInstances |
		aiProcess_ValidateDataStructure |
		aiProcess_OptimizeMeshes |
		aiProcess_Triangulate,
		NULL);

	if (!aScene)
	{
		std::cerr << "Failed to import" << loadInfo->meshPath << std::endl;
	}
	else if (aScene->HasMeshes())
	{
		assets.insert(std::pair<const std::string*, const aiScene*>(&(loadInfo->meshPath), aScene));
	}
}

MeshNode* MeshImporter::getMesh(MeshLoadInfo::LoadInfo* loadInfo)
{
	if (assets.count(&(loadInfo->meshPath)) == 0)
	{
		loadMesh(loadInfo);
	}
	const aiScene* scene = assets.find(&(loadInfo->meshPath))->second;
	return new MeshNode(generateUuid(), scene->mMeshes[0]);
}