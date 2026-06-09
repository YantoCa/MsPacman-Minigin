#pragma once
#include "Component.h"
#include "EventSystem.h"
#include "TextComponent.h"
#include "PointsComponent.h"
#include "GameObject.h"

namespace dae {
	class DisplayPointsComponent : public Component {
	public:
		DisplayPointsComponent(GameObject* owner, GameObject* player);

		void OnPointsChanged();
	private:
		void UpdateText();

		GameObject* m_Player;

		TextComponent* m_TextComponent{ nullptr };
		PointsComponent* m_PointsComponent{ nullptr };
	};
}