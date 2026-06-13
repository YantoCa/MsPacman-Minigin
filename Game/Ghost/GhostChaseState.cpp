#include "GhostChaseState.h"
#include "GameObject.h"
#include "GridMovementComponent.h"
#include "GridComponent.h"
#include "RenderComponent.h"
#include "GhostComponent.h"
#include "GameManager.h"
#include <climits>
#include <cmath>
#include <vector>

game::GhostChaseState::GhostChaseState(dae::GameObject& ghost, GameManager* pManager)
    : m_Ghost(ghost), m_pManager(pManager) {
}

void game::GhostChaseState::OnEnter()
{
    m_pGhostMove = m_Ghost.GetComponent<GridMovementComponent>();
    m_pGrid = m_pManager->GetMazeGrid();
     
    if (auto* ghostData = m_Ghost.GetComponent<GhostComponent>())
    {
        if (auto* render = m_Ghost.GetComponent<dae::RenderComponent>())
        {
            render->SetTexture(ghostData->GetNormalTexturePath());
        }
    }
}

void game::GhostChaseState::Update(float)
{
    glm::ivec2 currentTile = m_pGhostMove->GetCurrentTile();
     
    if (currentTile != m_LastDecisionTile)
    {
        ChooseNextDirection();
        m_LastDecisionTile = currentTile;
    }
}

void game::GhostChaseState::OnExit() {}

glm::ivec2 game::GhostChaseState::CalculateTargetTile() const
{
    auto* ghostData = m_Ghost.GetComponent<GhostComponent>();
    if (!ghostData) return { 0, 0 };

    dae::GameObject* pacman = ghostData->GetPacman();
    if (!pacman) return { 0, 0 };

    auto* pacmanMove = pacman->GetComponent<GridMovementComponent>();
    glm::ivec2 pacmanTile = pacmanMove->GetCurrentTile();
    glm::ivec2 pacmanDir = pacmanMove->GetCurrentDirection();

    switch (ghostData->GetType())
    {
    case GhostType::Blinky: 
        return pacmanTile;

    case GhostType::Pinky:  
        return pacmanTile + (pacmanDir * 4);

    case GhostType::Inky:  
    {
        dae::GameObject* blinky = ghostData->GetBlinky();
        if (!blinky) return pacmanTile;

        auto* blinkyMove = blinky->GetComponent<GridMovementComponent>();
        glm::ivec2 blinkyTile = blinkyMove->GetCurrentTile();

        glm::ivec2 intermediate = pacmanTile + (pacmanDir * 2);
        return blinkyTile + 2 * (intermediate - blinkyTile);
    }

    case GhostType::Sue:  
    {
        glm::ivec2 sueTile = m_pGhostMove->GetCurrentTile();
        float dx = static_cast<float>(sueTile.x - pacmanTile.x);
        float dy = static_cast<float>(sueTile.y - pacmanTile.y);
        float distance = std::hypot(dx, dy);

        if (distance > 8.0f) {
            return pacmanTile; 
        }
        else {
            return glm::ivec2{ 0, m_pGrid->GetRows() };  
        }
    }
    }
    return pacmanTile;
}

void game::GhostChaseState::ChooseNextDirection()
{
    glm::ivec2 currentTile = m_pGhostMove->GetCurrentTile();
    glm::ivec2 currentDir = m_pGhostMove->GetCurrentDirection();
    glm::ivec2 targetTile = CalculateTargetTile();

    std::vector<glm::ivec2> directions = { {0, -1}, {-1, 0}, {0, 1}, {1, 0} };  

    glm::ivec2 bestDir = currentDir;
    float shortestDistance = std::numeric_limits<float>::max();

    for (const auto& dir : directions)
    { 
        if (dir == -currentDir && currentDir != glm::ivec2{ 0, 0 }) continue;

        glm::ivec2 neighborTile = currentTile + dir;
 
        if (m_pGrid->IsWall(neighborTile.x, neighborTile.y)) continue;
         
        int dx = neighborTile.x - targetTile.x;
        int dy = neighborTile.y - targetTile.y;
        float distToTarget = static_cast<float>((dx * dx) + (dy * dy));
           
        if (distToTarget < shortestDistance)
        {
            shortestDistance = distToTarget;
            bestDir = dir;
        }
    }

    m_pGhostMove->SetDesiredDirection(bestDir);
}