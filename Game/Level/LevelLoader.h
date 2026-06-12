#pragma once
#include <string>
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

		static bool LoadLevel(const std::string& csvFilePath, dae::Scene& targetScene);
	private:
		static void SpawnTile(int tileId, int column, int row, dae::Scene& scene); 

		static constexpr float TILE_SIZE = 24.0f;

		static constexpr float PADDING_8x8 = 8.0f;
		static constexpr float PADDING_16x16 = 4.0f;
	};
}