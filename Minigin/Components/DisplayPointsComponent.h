#pragma once
#include "Component.h" 
#include "TextComponent.h"
#include "PointsComponent.h"
#include "GameObject.h"

#include "Observer.h"

namespace dae {
	class DisplayPointsComponent final : public Component, public Observer{
	public:
		DisplayPointsComponent(GameObject& owner); 
		~DisplayPointsComponent() override = default;

		void OnNotify(const GameObject& object, Event event) override;
	private: 
		TextComponent* m_pTextComponent{nullptr};
	};
}