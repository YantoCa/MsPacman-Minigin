#include "Commands.h"

#include "GameObject.h"
#include "Components/PointsComponent.h"
#include "Components/MovementComponent.h"

#include "ServiceLocator.h"
 
namespace game {
    // Add Points Command
    AddPointsCommand::AddPointsCommand(dae::GameObject& player, int points)
        : m_Player(player), m_Points(points) {
    }

    void AddPointsCommand::Execute() {
        auto points = m_Player.GetComponent<PointsComponent>();
        if (points)
        {
            points->AddPoints(m_Points);

            dae::ServiceLocator::GetSoundSystem().PlaySound(0, 0.3f);
        }
    }

    // MoveCommand 
    MoveCommand::MoveCommand(dae::GameObject& object, const glm::vec3& direction)
        : m_Object{ object }, m_Direction{ direction } {
    }

    void MoveCommand::Execute() {
        auto* movement = m_Object.GetComponent<MovementComponent>();

        if (movement)
        {
            movement->SetTargetDirection(m_Direction);
        }
    }
}