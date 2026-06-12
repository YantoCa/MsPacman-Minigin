#pragma once
#include "Component.h"
#include <vector>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

namespace game {
	class GridComponent final : public dae::Component {
	public:
		explicit GridComponent(dae::GameObject& owner, int columns, int rows, float tileSize = 8.f);
		virtual ~GridComponent() override = default;

		void SetTile(int column, int row, int tileId);
		int GetTileType(int column, int row) const;

		bool IsWall(int column, int row) const;

		//
		glm::ivec2 WorldToGrid(const glm::vec3& worldPos) const;
		glm::vec3 GridToWorldCenter(int column, int row) const;

		float GetTileSize() const;
		int GetColumns() const;
		int GetRows() const;
	private:
		int m_NumColumns;
		int m_NumRows;
		float m_TileSize;
		std::vector<std::vector<int>> m_Matrix;
	}; 
}