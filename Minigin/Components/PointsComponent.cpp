#include "PointsComponent.h"
#include "EventSystem.h"

namespace dae
{
    PointsComponent::PointsComponent(GameObject& owner)
        : Component(owner), m_Score(0)
    {
    }

    void PointsComponent::AddPoints(int amount)
    {
        m_Score += amount;

        EventSystem::GetInstance().Dispatch("PointsChanged");
    }

    int PointsComponent::GetScore() const
    {
        return m_Score;
    }
}