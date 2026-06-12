#pragma once
#include <string>
#include <glm/vec3.hpp>
#include "MsPacmanEnums.h"

namespace dae
{
	class Scene;
}

namespace game {
	class GridComponent;

	class LevelLoader final
	{
	public:
		LevelLoader() = delete;

		static GridComponent* LoadLevel(const std::string& csvFilePath, dae::Scene& targetScene);
	private:
		static std::vector<std::vector<TileType>> ParseCSV(const std::string& csvFilePath);
		
		static GridComponent* InitializeGrid(const std::vector<std::vector<TileType>>& matrix, dae::Scene& scene);

		static void PopulateScene(const std::vector<std::vector<TileType>>& matrix, dae::Scene& scene, GridComponent* pGrid);
		static void SpawnTile(TileType tileType, const glm::vec3& centerPos, dae::Scene& scene, GridComponent* pGrid);

		static constexpr float TILE_SIZE = 8.0f;
	};
}