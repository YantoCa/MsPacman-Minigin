#include "Commands.h"

#include "GameObject.h"
#include "GameManager.h"
#include "Components/GridMovementComponent.h"

#include "ServiceLocator.h"
 
namespace game {
    // SkipLevelCommand Command
    SkipLevelCommand::SkipLevelCommand(GameManager& object)
        : m_Object(object) {
    }

    void SkipLevelCommand::Execute() { 
        m_Object.NextLevelLogic();
    }

    // MoveCommand 
    MoveCommand::MoveCommand(dae::GameObject& object, const glm::ivec2& direction)
        : m_Object{ object }, m_Direction{ direction } {
    }

    void MoveCommand::Execute() {
        auto* movement = m_Object.GetComponent<GridMovementComponent>();

        if (movement)
        {
            movement->SetDesiredDirection(m_Direction);
        }
    }
}