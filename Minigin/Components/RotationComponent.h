#pragma once
#include "Component.h"

namespace dae
{
    class RotationComponent : public Component
    {
    public:
        RotationComponent(GameObject* owner, float speed = 90.0f, bool clockwise = true, float centerX = 0.0f, float centerY = 0.0f, float radius = 30.0f);

        void Update(float deltaTime) override;

        void SetRotationSpeed(float speed) { m_rotationSpeed = speed; }
        void SetClockwise(bool clockwise) { m_clockwise = clockwise; }
        void SetCenter(float x, float y) { m_centerX = x; m_centerY = y; }
        void SetRadius(float radius) { m_radius = radius; }

    private:
        float m_rotationSpeed;
        bool m_clockwise;
        float m_centerX, m_centerY;
        float m_radius;
        float m_angle;
    };
}