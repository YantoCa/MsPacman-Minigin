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

    class GhostFrightenedState final : public dae::State
    {
    public:
        GhostFrightenedState(dae::GameObject& ghost, GameManager* pManager);
        ~GhostFrightenedState() override = default;

        void OnEnter() override;
        void Update(float deltaTime) override;
        void OnExit() override;

    private:
        dae::GameObject& m_Ghost;
        GameManager* m_pManager;

        GridMovementComponent* m_pGhostMove{ nullptr };
        GridComponent* m_pGrid{ nullptr };

        glm::ivec2 m_LastDecisionTile{ -1, -1 };
        float m_FrightenedTimer{ 0.f };
        const float m_MaxFrightenedTime{ 7.0f };
    };
}