#pragma once
#include "Command.h"

#include <vector>
#include <glm/glm.hpp>
#include <memory>

// Iterations of Command
namespace game { 
    class MoveCommand final : public dae::Command {
    public:
        MoveCommand(dae::GameObject& object, const glm::ivec2& direction);

        void Execute() override;

    private:
        dae::GameObject& m_Object;

        glm::ivec2 m_Direction;
    };

    // delete seperate
    class AddPointsCommand final : public dae::Command
    {
    public:
        AddPointsCommand(dae::GameObject& player, int points);

        void Execute() override;
    private:
        dae::GameObject & m_Player;
        int m_Points;
    };
}