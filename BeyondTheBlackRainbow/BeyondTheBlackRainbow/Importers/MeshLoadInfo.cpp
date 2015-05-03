#include "MeshLoadInfo.h"
const MeshLoadInfo::ShaderLoadInfo* MeshLoadInfo::LIGHTING_SHADER = new ShaderLoadInfo(std::string("vertexShaderPath"), std::string("fragmentShaderPath"));
const MeshLoadInfo::ShaderLoadInfo* MeshLoadInfo::TEXTURE_SHADER = new ShaderLoadInfo(std::string("../BeyondTheBlackRainbow/Shader/Vertex/texture_vertex_shader.glsl"), std::string("../BeyondTheBlackRainbow/Shader/Fragment/texture_fragment_shader.glsl"));
const MeshLoadInfo::ShaderLoadInfo* MeshLoadInfo::SIMPLE_LIGHTING_SHADER = new ShaderLoadInfo(std::string("../BeyondTheBlackRainbow/Shader/Vertex/SimpleVertexShader.vertexshader"), std::string("../BeyondTheBlackRainbow/Shader/Fragment/SimpleFragmentShader.fragmentshader"));


const MeshLoadInfo::LoadInfo* MeshLoadInfo::TABLE = new LoadInfo(std::string("../BeyondTheBlackRainbow/Assets/Models/Table.dae"), std::string(""), LIGHTING_SHADER);
const MeshLoadInfo::LoadInfo* MeshLoadInfo::DUCK = new LoadInfo(std::string("../BeyondTheBlackRainbow/Assets/Models/duck.dae"), std::string("../BeyondTheBlackRainbow/Assets/Models/duck_textures/duck.png"), SIMPLE_LIGHTING_SHADER);