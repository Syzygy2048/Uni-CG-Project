#include "MeshLoadInfo.h"

const MeshLoadInfo::LoadInfo* MeshLoadInfo::TABLE = new LoadInfo(std::string("../BeyondTheBlackRainbow/Assets/Models/Table.dae"), std::string("") );
const MeshLoadInfo::LoadInfo* MeshLoadInfo::DUCK = new LoadInfo(std::string("../BeyondTheBlackRainbow/Assets/Models/duck.dae"), std::string("../BeyondTheBlackRainbow/Assets/Models/duck_textures/duck.png"));