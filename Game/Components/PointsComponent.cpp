#include "PointsComponent.h"
#include "Event.h"

namespace game
{
    PointsComponent::PointsComponent(dae::GameObject& owner)
        : Component(owner), m_Score(0)
    {
    }

    void PointsComponent::AddPoints(int amount)
    {
        m_Score += amount;
        
        Notify(*GetOwner(), dae::Event::ScoreChanged);
    }

    int PointsComponent::GetScore() const
    {
        return m_Score;
    }
}