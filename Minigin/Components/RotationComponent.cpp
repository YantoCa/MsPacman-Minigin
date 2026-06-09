#include "RotationComponent.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include <cmath>

namespace dae
{
    constexpr float PI = 3.14f;
    constexpr float DEG_TO_RAD = PI / 180.0f;

    RotationComponent::RotationComponent(GameObject* owner, float speed, float radius)
        : Component(owner), m_rotationSpeed(speed), m_radius(radius)
    {
    }

    void RotationComponent::Update(float deltaTime)
    {
        if (!GetOwner()) return;

        GameObject* owner = GetOwner(); 

        float deltaRadians = (m_rotationSpeed * DEG_TO_RAD) * deltaTime;
        m_angleInRadians += deltaRadians;  
        m_angleInRadians = std::fmod(m_angleInRadians, PI * 2.f); // wrap to avoid floating point errors over time
        if (m_angleInRadians < 0.f) {
            m_angleInRadians += PI * 2.f;
        }

        float x = m_radius * std::cos(m_angleInRadians);
        float y = m_radius * std::sin(m_angleInRadians);

        owner->GetTransform().SetLocalPosition({ x, y, 0.0f });
    }

}