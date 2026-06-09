#include "DisplayPointsComponent.h"

namespace dae {
	DisplayPointsComponent::DisplayPointsComponent(GameObject* owner, GameObject* player)
        : Component(owner), m_Player(player)
    {
        m_TextComponent = owner->GetComponent<TextComponent>();
        m_PointsComponent = player->GetComponent<PointsComponent>();

        UpdateText();

        EventSystem::GetInstance().Subscribe("PointsChanged", [this]() { OnPointsChanged(); });
    }

	void DisplayPointsComponent::OnPointsChanged(){
        if (m_PointsComponent) {
            UpdateText();
        }
    }

	void DisplayPointsComponent::UpdateText(){
        if (m_TextComponent)
            m_TextComponent->SetText("Score: " + std::to_string(m_PointsComponent->GetScore()));
    }
}