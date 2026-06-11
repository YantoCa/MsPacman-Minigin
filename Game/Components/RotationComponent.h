#pragma once
#include "Component.h"

namespace game
{
    class RotationComponent final : public dae::Component
    {
    public:
        RotationComponent(dae::GameObject& owner, float speed = 90.0f, float radius = 30.0f);

        void Update(float deltaTime) override;

        void SetRotationSpeed(float speed) { m_rotationSpeed = speed; }  
        void SetRadius(float radius) { m_radius = radius; }

    private:
        float m_rotationSpeed;  
        float m_radius;
        float m_angleInRadians{};
    };
}