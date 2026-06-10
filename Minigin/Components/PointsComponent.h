#pragma once
#include "Component.h"

namespace dae
{
    class PointsComponent final : public Component
    {
    public:
        PointsComponent(GameObject& owner);

        void AddPoints(int amount);
        int GetScore() const;

    private:
        int m_Score;
    };
}