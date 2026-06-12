#pragma once
#include "Component.h"
#include <glm/vec3.hpp>

namespace dae {
    class BoxColliderComponent final : public Component
    {
    public: 
        explicit BoxColliderComponent(GameObject& pOwner, float width, float height, float depth = 1.f, const glm::vec3& offset = { 0.f, 0.f, 0.f});
        ~BoxColliderComponent() override = default;

        bool IsOverlapping(const BoxColliderComponent& other) const;

        glm::vec3 GetWorldCenter() const;
        glm::vec3 GetHalfExtents() const { return m_HalfExtents; }

    private:
        glm::vec3 m_HalfExtents;
        glm::vec3 m_Offset;     
    };
}