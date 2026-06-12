#include "GridComponent.h"

using namespace game;

GridComponent::GridComponent(dae::GameObject& owner, int columns, int rows, float tileSize)
	: dae::Component(owner), m_NumColumns{ columns }, m_NumRows{ rows }, m_TileSize{ tileSize }{
	//
	m_Matrix.resize(m_NumRows, std::vector<int>(m_NumColumns, 0));
}
void GridComponent::SetTile(int column, int row, int tileId) {
	if (row >= 0 && row < m_NumRows && column >= 0 && column < m_NumColumns)
	{
		m_Matrix[row][column] = tileId;
	}
}
int GridComponent::GetTileType(int column, int row) const {
	if (row >= 0 && row < m_NumRows && column >= 0 && column < m_NumColumns)
	{
		return m_Matrix[row][column];
	}
	return -1; // fallback (out of bounds)
}

bool GridComponent::IsWall(int column, int row) const {
	return GetTileType(column, row) == 1;
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