#pragma once
#include "Component.h" 
#include "TextComponent.h"
#include "PointsComponent.h"
#include "GameObject.h"

#include "Observer.h"

namespace game {
	class DisplayPointsComponent final : public dae::Component, public dae::Observer{
	public:
		DisplayPointsComponent(dae::GameObject& owner);
		~DisplayPointsComponent() override = default;

		void OnNotify(const dae::GameObject& object, dae::Event event) override;
	private: 
		dae::TextComponent* m_pTextComponent{nullptr};
	};
}