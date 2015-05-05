#pragma once
#include <iostream>
class MeshLoadInfo{
public:
	struct ShaderLoadInfo {
		const std::string vertexShaderPath;
		const std::string fragmentShaderPath;
		ShaderLoadInfo(const std::string& vShaderPath, const std::string& fShaderPath) : fragmentShaderPath(fShaderPath), vertexShaderPath(vShaderPath){}
	};
	static const ShaderLoadInfo* LIGHTING_SHADER;
	static const ShaderLoadInfo* TEXTURE_SHADER;
	static const ShaderLoadInfo* SIMPLE_LIGHTING_SHADER;

	struct LoadInfo {
		const std::string meshPath;
		const std::string texturePath;
		const ShaderLoadInfo* shaderInfo;
		LoadInfo(const std::string& mPath, const std::string& tPath, const ShaderLoadInfo* sInfo) : meshPath(mPath), texturePath(tPath), shaderInfo(sInfo){}
	};
	static const LoadInfo* TABLE;
	static const LoadInfo* DUCK;
	static const LoadInfo* BED;
	static const LoadInfo* ROOM;
	static const LoadInfo* DOOR;
};