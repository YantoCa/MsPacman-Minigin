#pragma once
#include <string> 
#include "MsPacmanEnums.h"
#include <vector>

namespace dae
{
	class Scene;
}

namespace game { 
	class LevelLoader final
	{
	public:
		LevelLoader() = delete;

		static std::vector<std::vector<TileType>> ParseCSV(const std::string& csvFilePath);
	};
}