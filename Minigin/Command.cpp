#include "Command.h"

#include "GameObject.h"
#include "Components/PointsComponent.h"
#include "Components/MovementComponent.h"

namespace dae {
	// Add Points Command
    AddPointsCommand::AddPointsCommand(GameObject& player, int points)
        : m_Player(player), m_Points(points) {
    }

    void AddPointsCommand::Execute(){
        auto points = m_Player.GetComponent<PointsComponent>();
        if (points)
        {
            points->AddPoints(m_Points);
        }
    } 

    // MoveCommand 
    MoveCommand::MoveCommand(GameObject& object, const glm::vec3& direction)
        : m_Object{ object }, m_Direction{ direction } { }

    void MoveCommand::Execute() {
        auto* movement = m_Object.GetComponent<MovementComponent>();
         
        if (movement)
        {
            movement->SetTargetDirection(m_Direction);
        }
    }
}