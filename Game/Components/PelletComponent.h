#pragma once
#include "Component.h"
#include "Subject.h"

namespace game {
    class PelletComponent final : public dae::Component , public dae::Subject{
    public:
        explicit PelletComponent(dae::GameObject& owner, int pointValue, bool isPowerPellet = false);
        ~PelletComponent() override = default;

        void Eat();
        void Reset();

        bool IsEaten() const { return m_IsEaten; }
        int GetPointValue() const { return m_PointValue; }
        bool IsPowerPellet() const { return m_IsPowerPellet; }
    private:
        int m_PointValue;
        bool m_IsPowerPellet;
        bool m_IsEaten{ false };
    };
}