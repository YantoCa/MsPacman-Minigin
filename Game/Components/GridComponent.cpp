#include "GridComponent.h"

using namespace game;

GridComponent::GridComponent(dae::GameObject& owner, int columns, int rows, float tileSize)
	: dae::Component(owner), m_NumColumns{ columns }, m_NumRows{ rows }, m_TileSize{ tileSize }{
	//
	m_Grid.resize(m_NumRows, std::vector<TileType>(m_NumColumns, TileType::Empty));
}
void GridComponent::SetTile(int column, int row, TileType type) {
	if (row >= 0 && row < m_NumRows && column >= 0 && column < m_NumColumns)
	{
		m_Grid[row][column] = type;
	}
}
TileType GridComponent::GetTileType(int column, int row) const {
	if (row >= 0 && row < m_NumRows && column >= 0 && column < m_NumColumns)
	{
		return m_Grid[row][column];
	}
	return TileType::Wall; // fallback (out of bounds) keeps the player inside
}

bool GridComponent::IsWall(int column, int row) const {
	return GetTileType(column, row) == TileType::Wall;
}

glm::ivec2 GridComponent::WorldToGrid(const glm::vec3& worldPos) const {
	return {
		static_cast<int>(worldPos.x / m_TileSize),
		static_cast<int>(worldPos.y / m_TileSize)
	};
}
glm::vec3 GridComponent::GridToWorldCenter(int column, int row) const {
	float x = (column * m_TileSize) + (m_TileSize / 2.0f);
	float y = (row * m_TileSize) + (m_TileSize / 2.0f);
	return { x, y, 0.0f };
}

float GridComponent::GetTileSize() const { return m_TileSize; }
int GridComponent::GetColumns() const { return m_NumColumns; }
int GridComponent::GetRows() const { return m_NumRows; }


void GridComponent::AddPlayerSpawnPoint(const glm::vec3& position) { m_PlayerSpawnPoints.push_back(position); }
void GridComponent::AddGhostSpawnPoint(const glm::vec3& position) { m_GhostSpawnPoints.push_back(position); }

const std::vector<glm::vec3>& GridComponent::GetPlayerSpawnPoints() const { return m_PlayerSpawnPoints; }
const std::vector<glm::vec3>& GridComponent::GetGhostSpawnPoints() const { return m_GhostSpawnPoints; }