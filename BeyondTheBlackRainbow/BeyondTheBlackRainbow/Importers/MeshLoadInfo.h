#pragma once

#include <iostream>

class MeshLoadInfo{
public:
	struct LoadInfo {
		const std::string meshPath;
		const std::string texturePath;
		//shader list

		LoadInfo(const std::string& mPath, const std::string& tPath) : meshPath(mPath), texturePath(tPath){  };
	};

	LoadInfo TABLE {
		"../Assets/Models/Table.dae",
		""
	};
	LoadInfo CHAIR {
		"",
		""
	};
};