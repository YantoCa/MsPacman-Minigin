#include "GridMovementComponent.h"
#include "GameObject.h"
#include "GridComponent.h"

using namespace game;

GridMovementComponent::GridMovementComponent(dae::GameObject& pOwner, GridComponent* pGrid, float speed)
	: dae::Component(pOwner)
	, m_pGrid(pGrid)
	, m_Speed(speed)
{
	if (m_pGrid)
	{
		SnapToGrid();
	}
}

void GridMovementComponent::SnapToGrid()
{
	auto& transform = GetOwner()->GetTransform();

	m_CurrentTile = m_pGrid->WorldToGrid(transform.GetWorldPosition());
	m_TargetTile = m_CurrentTile;

	transform.SetWorldPosition(m_pGrid->GridToWorldCenter(m_CurrentTile.x, m_CurrentTile.y));
	m_IsMovingToTarget = false;
}

void GridMovementComponent::Update(float deltaTime)
{
	auto& transform = GetOwner()->GetTransform();
	glm::vec3 currentPos = transform.GetWorldPosition();

	if (!m_IsMovingToTarget)
	{ 
		if (m_DesiredDirection != glm::ivec2{ 0, 0 })
		{
			glm::ivec2 nextTileCheck = m_CurrentTile + m_DesiredDirection;
			if (!m_pGrid->IsWall(nextTileCheck.x, nextTileCheck.y))
			{
				m_CurrentDirection = m_DesiredDirection;
				m_TargetTile = nextTileCheck;
				m_IsMovingToTarget = true;
			}
		}
		 
		if (!m_IsMovingToTarget && m_CurrentDirection != glm::ivec2{ 0, 0 })
		{
			glm::ivec2 nextTileCheck = m_CurrentTile + m_CurrentDirection;
			if (!m_pGrid->IsWall(nextTileCheck.x, nextTileCheck.y))
			{
				m_TargetTile = nextTileCheck;
				m_IsMovingToTarget = true;
			}
			else
			{
				m_CurrentDirection = { 0, 0 }; // Stop
			}
		}
	}
	 
	if (m_IsMovingToTarget)
	{
		glm::vec3 targetWorldPos = m_pGrid->GridToWorldCenter(m_TargetTile.x, m_TargetTile.y);
		glm::vec3 moveVector = targetWorldPos - currentPos;

		float distanceToTarget = glm::length(moveVector);
		float moveStep = m_Speed * deltaTime;

		if (moveStep >= distanceToTarget)
		{ 
			transform.SetWorldPosition(targetWorldPos);
			m_CurrentTile = m_TargetTile;
			m_IsMovingToTarget = false;
		}
		else
		{ 
			transform.SetWorldPosition(currentPos + (glm::normalize(moveVector) * moveStep));
		}
	}
}
void   GridMovementComponent::SetDesiredDirection(const glm::ivec2& dir) { m_DesiredDirection = dir; }

void  GridMovementComponent::SetGrid(GridComponent* pGrid) { // Update to new grid (when level ends)
	m_pGrid = pGrid;

	if (m_pGrid)
	{
		SnapToGrid();
	}
}