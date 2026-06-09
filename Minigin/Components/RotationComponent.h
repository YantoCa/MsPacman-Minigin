#pragma once
#include "Component.h"

namespace dae
{
    class RotationComponent : public Component
    {
    public:
        RotationComponent(GameObject* owner, float speed = 90.0f, float radius = 30.0f);

        void Update(float deltaTime) override;

        void SetRotationSpeed(float speed) { m_rotationSpeed = speed; }  
        void SetRadius(float radius) { m_radius = radius; }

    private:
        float m_rotationSpeed;  
        float m_radius;
        float m_angleInRadians{};
    };
}