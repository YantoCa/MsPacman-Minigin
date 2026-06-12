#pragma once
#include "Component.h"
#include <vector>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include "MsPacmanEnums.h"

namespace game {
	constexpr float TILE_SIZE = 8.f;

	class GridComponent final : public dae::Component {
	public:
		explicit GridComponent(dae::GameObject& owner, int columns, int rows, float tileSize = 8.f);
		virtual ~GridComponent() override = default;

		void SetTile(int column, int row, TileType type);
		TileType GetTileType(int column, int row) const;

		bool IsWall(int column, int row) const;
		
		glm::ivec2 WorldToGrid(const glm::vec3& worldPos) const;
		glm::vec3 GridToWorldCenter(int column, int row) const;

		void AddPlayerSpawnPoint(const glm::vec3& position);
		void AddGhostSpawnPoint(const glm::vec3& position);

		const std::vector<glm::vec3>& GetPlayerSpawnPoints() const;
		const std::vector<glm::vec3>& GetGhostSpawnPoints() const;

		float GetTileSize() const;
		int GetColumns() const;
		int GetRows() const;
	private:
		int m_NumColumns;
		int m_NumRows;
		float m_TileSize;
		std::vector<std::vector<TileType>> m_Grid{};

		std::vector<glm::vec3> m_PlayerSpawnPoints{};
		std::vector<glm::vec3> m_GhostSpawnPoints{};
	}; 
}