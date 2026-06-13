#pragma once
#include "State.h"
#include <glm/vec2.hpp>

namespace dae { 
    class GameObject; 
} 

namespace game
{
    class GridMovementComponent; 
    class GridComponent; 
    class GameManager;

    class GhostChaseState final : public dae::State
    {
    public:
        GhostChaseState(dae::GameObject& ghost, GameManager* pManager);
        ~GhostChaseState() override = default;

        void OnEnter() override;
        void Update(float deltaTime) override;
        void OnExit() override;

    private:
        dae::GameObject& m_Ghost;
        GameManager* m_pManager;

        GridMovementComponent* m_pGhostMove{ nullptr };
        GridComponent* m_pGrid{ nullptr };

        glm::ivec2 m_LastDecisionTile{ -1, -1 };

        glm::ivec2 CalculateTargetTile() const;
        void ChooseNextDirection();
    };
}