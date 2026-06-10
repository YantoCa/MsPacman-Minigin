#include "PointsComponent.h"
#include "Event.h"

namespace dae
{
    PointsComponent::PointsComponent(GameObject& owner)
        : Component(owner), m_Score(0)
    {
    }

    void PointsComponent::AddPoints(int amount)
    {
        m_Score += amount;
        
        Notify(*GetOwner(), Event::ScoreChanged);
    }

    int PointsComponent::GetScore() const
    {
        return m_Score;
    }
}