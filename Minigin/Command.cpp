#include "Command.h"

#include "GameObject.h"
#include "PointsComponent.h"

namespace dae {
	// Add Points Command
    AddPointsCommand::AddPointsCommand(std::shared_ptr<GameObject> player, int points)
        : m_Player(player), m_Points(points) {
    }

    void AddPointsCommand::Execute(){
        if (m_Player)
        {
            auto points = m_Player->GetComponent<PointsComponent>();
            if (points)
            {
                points->AddPoints(m_Points);
            }
        }
    }

    //
}