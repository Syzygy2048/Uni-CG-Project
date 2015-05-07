#include "MeshLoadInfo.h"
const MeshLoadInfo::ShaderLoadInfo* MeshLoadInfo::LIGHTING_SHADER = new ShaderLoadInfo(std::string("vertexShaderPath"), std::string("fragmentShaderPath"));
const MeshLoadInfo::ShaderLoadInfo* MeshLoadInfo::TEXTURE_SHADER = new ShaderLoadInfo(std::string("../BeyondTheBlackRainbow/Shader/Vertex/texture_vertex_shader.glsl"), std::string("../BeyondTheBlackRainbow/Shader/Fragment/texture_fragment_shader.glsl"));
const MeshLoadInfo::ShaderLoadInfo* MeshLoadInfo::SIMPLE_LIGHTING_SHADER = new ShaderLoadInfo(std::string("../BeyondTheBlackRainbow/Shader/Vertex/SimpleVertexShader.vertexshader"), std::string("../BeyondTheBlackRainbow/Shader/Fragment/SimpleFragmentShader.fragmentshader"));


const MeshLoadInfo::LoadInfo* MeshLoadInfo::TABLE = new LoadInfo(std::string("../BeyondTheBlackRainbow/Assets/Models/Table.obj"), std::string("../BeyondTheBlackRainbow/Assets/Models/duck_textures/wood _texture.jpg"), SIMPLE_LIGHTING_SHADER, MeshLoadInfo::CollisionType::DYNAMIC, MeshLoadInfo::CollisionShapeType::CONVEX);
const MeshLoadInfo::LoadInfo* MeshLoadInfo::ROOM = new LoadInfo(std::string("../BeyondTheBlackRainbow/Assets/Models/Room.obj"), std::string("../BeyondTheBlackRainbow/Assets/Models/duck_textures/room.png"), SIMPLE_LIGHTING_SHADER, MeshLoadInfo::CollisionType::STATIC, MeshLoadInfo::CollisionShapeType::TRIANGLE);
const MeshLoadInfo::LoadInfo* MeshLoadInfo::BED = new LoadInfo(std::string("../BeyondTheBlackRainbow/Assets/Models/Bed.obj"), std::string("../BeyondTheBlackRainbow/Assets/Models/duck_textures/whiteBed.jpg"), SIMPLE_LIGHTING_SHADER, MeshLoadInfo::CollisionType::DYNAMIC, MeshLoadInfo::CollisionShapeType::CONVEX);
const MeshLoadInfo::LoadInfo* MeshLoadInfo::DOOR = new LoadInfo(std::string("../BeyondTheBlackRainbow/Assets/Models/Door.obj"), std::string("../BeyondTheBlackRainbow/Assets/Models/duck_textures/door.png"), SIMPLE_LIGHTING_SHADER, MeshLoadInfo::CollisionType::DYNAMIC, MeshLoadInfo::CollisionShapeType::CONVEX);
const MeshLoadInfo::LoadInfo* MeshLoadInfo::DUCK = new LoadInfo(std::string("../BeyondTheBlackRainbow/Assets/Models/duck.dae"), std::string("../BeyondTheBlackRainbow/Assets/Models/duck_textures/duck.png"), SIMPLE_LIGHTING_SHADER, MeshLoadInfo::CollisionType::DYNAMIC, MeshLoadInfo::CollisionShapeType::SPHERE);