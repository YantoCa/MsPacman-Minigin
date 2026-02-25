#include "RotationComponent.h"
#include "GameObject.h"
#include "Transform.h"
#include <cmath>

namespace dae
{
    constexpr float PI = 3.14f;
    constexpr float DEG_TO_RAD = PI / 180.0f;

    RotationComponent::RotationComponent(GameObject* owner, float speed, bool clockwise, float centerX, float centerY, float radius)
        : Component(owner), m_rotationSpeed(speed), m_clockwise(clockwise), m_centerX(centerX), m_centerY(centerY), m_radius(radius), m_angle(0.0f)
    {
        if (centerX == 0.0f && centerY == 0.0f)
        {
            m_centerX = owner->GetTransform().GetWorldPosition().x;
            m_centerY = owner->GetTransform().GetWorldPosition().y;
        }
    }

    void RotationComponent::Update(float deltaTime)
    {
        if (!GetOwner()) return;

        GameObject* owner = GetOwner();
        GameObject* parent = owner->GetParent();

        float deltaAngle = m_rotationSpeed * deltaTime;
        m_angle += m_clockwise ? deltaAngle : -deltaAngle;
        float radians = m_angle * DEG_TO_RAD;

        if (parent)
        {
            glm::vec3 parentPos = parent->GetTransform().GetWorldPosition();
            float x = parentPos.x + m_radius * std::cos(radians);
            float y = parentPos.y + m_radius * std::sin(radians);

            owner->GetTransform().SetWorldPosition({ x, y, 0.0f });
        }
        else
        {
            float x = m_centerX + m_radius * std::cos(radians);
            float y = m_centerY + m_radius * std::sin(radians);

            owner->GetTransform().SetLocalPosition({ x, y, 0.0f });
        }
    }

}