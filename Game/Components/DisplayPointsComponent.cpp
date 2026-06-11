#include "DisplayPointsComponent.h" 
#include "Components/Component.h"
#include "Components/TextComponent.h" 
#include "GameObject.h"  
#include "Event.h"

namespace dae {
	DisplayPointsComponent::DisplayPointsComponent(GameObject& owner)
        : Component(owner)
    {
        m_pTextComponent = GetOwner()->GetComponent<TextComponent>();
        
    }
 
    void DisplayPointsComponent::OnNotify(const GameObject& object, Event event) {
        if (event == Event::ScoreChanged) {
            auto* pointsComp = object.GetComponent<PointsComponent>();
            // Always safety check your pointers!
            if (pointsComp && m_pTextComponent) {
                m_pTextComponent->SetText("Score: " + std::to_string(pointsComp->GetScore()));
            }
        }
    }
}