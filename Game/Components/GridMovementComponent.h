#pragma once
#include "Component.h"
#include <glm/vec2.hpp>

namespace game
{
	class GridComponent;

	class GridMovementComponent final : public dae::Component
	{
	public:
		explicit GridMovementComponent(dae::GameObject& pOwner, GridComponent* pGrid, float speed = 60.0f);
		virtual ~GridMovementComponent() override = default;

		virtual void Update(float deltaTime) override;
		 
		void SetDesiredDirection(const glm::ivec2& dir);
		void SetGrid(GridComponent* pGrid);
		void SnapToGrid();

		glm::vec2 GetCurrentTile() const { return m_CurrentTile; }
		glm::vec2 GetCurrentDirection() const { return m_CurrentDirection; }
	private:
		GridComponent* m_pGrid;
		float m_Speed;

		glm::ivec2 m_CurrentDirection{ 0, 0 };
		glm::ivec2 m_DesiredDirection{ 0, 0 };

		glm::ivec2 m_CurrentTile{ 0, 0 };
		glm::ivec2 m_TargetTile{ 0, 0 };
		bool m_IsMovingToTarget{ false };

	};
}