#include "FPSComponent.h"
#include "GameObject.h"

#include <sstream>
#include <iomanip>
#include <algorithm> 

namespace game {
    FPSComponent::FPSComponent(dae::GameObject& owner, std::shared_ptr<dae::Font> font)
        : Component(owner)
    {
        m_TextComponent = GetOwner()->GetComponent<dae::TextComponent>();

        if (!m_TextComponent) m_TextComponent = GetOwner()->AddComponent<dae::TextComponent>("FPS: 0", font);

    }

    void FPSComponent::Update(float deltaTime)
    {
        if (!m_TextComponent || deltaTime <= 0.0f)
            return;

        m_AccumulatedTime += deltaTime;

        if (m_AccumulatedTime >= 1.0f)
        {
            float avgFPS = 1 / deltaTime;
            avgFPS = std::min(avgFPS, 240.f); // Cap the FPS to 240

            std::stringstream fpsStream;
            fpsStream << std::fixed << std::setprecision(1) << avgFPS;

            m_TextComponent->SetText(fpsStream.str() + " FPS");

            m_AccumulatedTime -= 1.0f;
        }
    }
}