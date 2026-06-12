#include "DisplayPointsComponent.h" 
#include "Components/Component.h"
#include "Components/TextComponent.h" 
#include "GameObject.h"  
#include "Events.h"

namespace game {
	DisplayPointsComponent::DisplayPointsComponent(dae::GameObject& owner)
        : Component(owner)
    {
        m_pTextComponent = GetOwner()->GetComponent<dae::TextComponent>();
        
    }
 
    void DisplayPointsComponent::OnNotify(const dae::GameObject& object, const std::string& event) {
        if (event == game::Event::ScoreChanged) {
            auto* pointsComp = object.GetComponent<PointsComponent>();
            // Always safety check your pointers!
            if (pointsComp && m_pTextComponent) {
                m_pTextComponent->SetText("Score: " + std::to_string(pointsComp->GetScore()));
            }
        }
    }
}