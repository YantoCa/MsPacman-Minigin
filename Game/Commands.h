#pragma once
#include "Command.h"

#include <vector>
#include <glm/glm.hpp>
#include <memory>

// Iterations of Command
class MoveCommand final : public dae::Command {
public:
    MoveCommand(dae::GameObject& object, const glm::vec3& direction);

    void Execute() override;

private:
    dae::GameObject& m_Object;

    glm::vec3 m_Direction;
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