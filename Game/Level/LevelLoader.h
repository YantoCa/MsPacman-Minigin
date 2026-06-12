#pragma once
#include <string>
#include <glm/vec3.hpp>

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

		static bool LoadLevel(const std::string& csvFilePath, dae::Scene& targetScene);
	private:
		static std::vector<std::vector<int>> ParseCSV(const std::string& csvFilePath);
		
		static GridComponent* InitializeGrid(const std::vector<std::vector<int>>& matrix, dae::Scene& scene);

		static void PopulateScene(const std::vector<std::vector<int>>& matrix, dae::Scene& scene, GridComponent* pGrid);
		static void SpawnTile(int tileId, const glm::vec3& centerPos, dae::Scene& scene, GridComponent* m_pGrid);

		static constexpr float TILE_SIZE = 8.0f;
	};
}