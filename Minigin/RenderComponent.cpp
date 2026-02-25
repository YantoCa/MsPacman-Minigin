#include "RenderComponent.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "GameObject.h"

namespace dae {
    RenderComponent::RenderComponent(GameObject* owner, const std::string& filename)
        : Component(owner)
    {
        SetTexture(filename);
    }

    void RenderComponent::Render() const
    {
        if (!m_Texture || !GetOwner())
            return;

        const auto& worldPos = GetOwner()->GetTransform().GetPosition();
        Renderer::GetInstance().RenderTexture(*m_Texture, worldPos.x, worldPos.y);
    }

    void RenderComponent::SetTexture(const std::string& filename)
    {
        m_Texture = ResourceManager::GetInstance().LoadTexture(filename);
    }
}