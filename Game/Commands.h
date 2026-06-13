#pragma once
#include "Command.h"

#include <vector>
#include <glm/glm.hpp>
#include <memory>

// Iterations of Command
namespace game { 
    // MoveCommand
    class MoveCommand final : public dae::Command {
    public:
        MoveCommand(dae::GameObject& object, const glm::ivec2& direction);

        void Execute() override;

    private:
        dae::GameObject& m_Object;

        glm::ivec2 m_Direction;
    };

    // SkipLevelCommand
    class GameManager;
    class SkipLevelCommand final : public dae::Command
    {
    public:
        SkipLevelCommand(GameManager& player);

        void Execute() override;
    private:
        GameManager& m_Object;
    };

    // MuteCommand
    class MuteCommand final : public dae::Command {
    public:
        MuteCommand() = default;

        void Execute() override;
    };
}