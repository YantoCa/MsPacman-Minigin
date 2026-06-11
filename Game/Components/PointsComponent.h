#pragma once
#include "Component.h"
#include "Subject.h"

namespace dae
{
    class PointsComponent final : public Component, public Subject
    {
    public:
        PointsComponent(GameObject& owner);

        void AddPoints(int amount);
        int GetScore() const;

    private:
        int m_Score;
    };
}