#include "GridComponent.h"
#include "GameObject.h"

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
	glm::vec3 gridOffset = GetOwner()->GetTransform().GetWorldPosition();
	 
	glm::vec3 localPos = worldPos - gridOffset;

	int column = static_cast<int>(localPos.x / m_TileSize);
	int row = static_cast<int>(localPos.y / m_TileSize);

	return glm::ivec2(column, row);
}
glm::vec3 GridComponent::GridToWorldCenter(int column, int row) const { 
	float localX = column * m_TileSize + (m_TileSize / 2.f);
	float localY = row * m_TileSize + (m_TileSize / 2.f);
	 
	glm::vec3 gridOffset = GetOwner()->GetTransform().GetWorldPosition();
	 
	return gridOffset + glm::vec3(localX, localY, 0.f);
}

float GridComponent::GetTileSize() const { return m_TileSize; }
int GridComponent::GetColumns() const { return m_NumColumns; }
int GridComponent::GetRows() const { return m_NumRows; }


void GridComponent::AddPlayerSpawnPoint(const glm::vec3& position) { m_PlayerSpawnPoints.push_back(position); }
void GridComponent::AddGhostSpawnPoint(const glm::vec3& position) { m_GhostSpawnPoints.push_back(position); }

const std::vector<glm::vec3>& GridComponent::GetPlayerSpawnPoints() const { return m_PlayerSpawnPoints; }
const std::vector<glm::vec3>& GridComponent::GetGhostSpawnPoints() const { return m_GhostSpawnPoints; }