#pragma once
#include "Component.h"
#include "Subject.h"

namespace game
{
    class PointsComponent final : public dae::Component, public dae::Subject
    {
    public:
        PointsComponent(dae::GameObject& owner);

        void AddPoints(int amount);
        int GetScore() const;

    private:
        int m_Score;
    };
}