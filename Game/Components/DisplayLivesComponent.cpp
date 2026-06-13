#include "DisplayLivesComponent.h" 
#include "Components/Component.h"
#include "Components/TextComponent.h" 
#include "GameObject.h"  
#include "Events.h"
#include "GameManager.h"

namespace game {
    DisplayLivesComponent::DisplayLivesComponent(dae::GameObject& owner)
        : Component(owner)
    {
        m_pTextComponent = GetOwner()->GetComponent<dae::TextComponent>();

    }

    void DisplayLivesComponent::OnNotify(const dae::GameObject& object, const std::string& event) {
        if (event == game::Event::LivesChanged) {
            auto* manager = object.GetComponent<GameManager>();
            int currentLives = manager->GetLives();

            m_pTextComponent->SetText("Lives: " + std::to_string(currentLives));
        }
    }
}