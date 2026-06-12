#include "BoxColliderComponent.h"
#include "GameObject.h"

using namespace dae;

BoxColliderComponent::BoxColliderComponent(GameObject& pOwner, float width, float height, float depth, const glm::vec3& offset)
    : Component(pOwner)
    , m_HalfExtents(width / 2.f, height / 2.f, depth / 2.f)
    , m_Offset(offset)
{
}

glm::vec3 BoxColliderComponent::GetWorldCenter() const
{
    auto worldPos = GetOwner()->GetTransform().GetWorldPosition();
    return worldPos + m_Offset;
}

bool BoxColliderComponent::IsOverlapping(const BoxColliderComponent& other) const
{
    glm::vec3 centerA = this->GetWorldCenter();
    glm::vec3 centerB = other.GetWorldCenter();
     
    float deltaX = std::abs(centerA.x - centerB.x);
    float deltaY = std::abs(centerA.y - centerB.y);
    float deltaZ = std::abs(centerA.z - centerB.z);
     
    float sumHalfWidths = m_HalfExtents.x + other.m_HalfExtents.x;
    float sumHalfHeights = m_HalfExtents.y + other.m_HalfExtents.y;
    float sumHalfDepths = m_HalfExtents.z + other.m_HalfExtents.z;
     
    return (deltaX < sumHalfWidths) &&
        (deltaY < sumHalfHeights) &&
        (deltaZ < sumHalfDepths);
}