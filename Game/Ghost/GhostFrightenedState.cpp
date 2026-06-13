#include "GhostFrightenedState.h"
#include "GameObject.h"
#include "GridMovementComponent.h"
#include "GridComponent.h"
#include "RenderComponent.h"
#include "GhostComponent.h"
#include "GameManager.h"
#include "FSMComponent.h"
#include "GhostChaseState.h"
#include <vector>
#include <cstdlib> 

game::GhostFrightenedState::GhostFrightenedState(dae::GameObject& ghost, GameManager* pManager)
    : m_Ghost(ghost), m_pManager(pManager) {
}

void game::GhostFrightenedState::OnEnter()
{
    m_pGhostMove = m_Ghost.GetComponent<GridMovementComponent>();
    m_pGrid = m_pManager->GetMazeGrid();
     
    if (auto* render = m_Ghost.GetComponent<dae::RenderComponent>()) {
        render->SetTexture(GhostComponent::GetFrightenedTexturePath());
    }
     
    glm::ivec2 currentDir = m_pGhostMove->GetCurrentDirection();
    m_pGhostMove->SetDesiredDirection(-currentDir);
}

void game::GhostFrightenedState::Update(float deltaTime)
{ 
    m_FrightenedTimer += deltaTime;
    if (m_FrightenedTimer >= m_MaxFrightenedTime)
    {
        if (auto* fsm = m_Ghost.GetComponent<dae::FSMComponent>())
        { 
            fsm->ChangeState(std::make_unique<GhostChaseState>(m_Ghost, m_pManager));
        }
        return;
    }

    glm::ivec2 currentTile = m_pGhostMove->GetCurrentTile();
    if (currentTile != m_LastDecisionTile)
    {
        glm::ivec2 currentDir = m_pGhostMove->GetCurrentDirection();
        std::vector<glm::ivec2> validDirections;
        std::vector<glm::ivec2> options = { {0, -1}, {-1, 0}, {0, 1}, {1, 0} };

        for (const auto& dir : options)
        {
            if (dir == -currentDir && currentDir != glm::ivec2{ 0, 0 }) continue;

            glm::ivec2 neighbor = currentTile + dir;
            if (!m_pGrid->IsWall(neighbor.x, neighbor.y)) {
                validDirections.push_back(dir);
            }
        }
         
        if (!validDirections.empty()) {
            int index = std::rand() % validDirections.size();
            m_pGhostMove->SetDesiredDirection(validDirections[index]);
        }

        m_LastDecisionTile = currentTile;
    }
}

void game::GhostFrightenedState::OnExit() {}