#pragma once
#include "Component.h" 
#include "TextComponent.h" 
#include "GameObject.h"

#include "Observer.h"

namespace game {
	class DisplayLivesComponent final : public dae::Component, public dae::Observer {
	public:
		DisplayLivesComponent(dae::GameObject& owner);
		~DisplayLivesComponent() override = default;

		void OnNotify(const dae::GameObject& object, const std::string& event) override;
	private:
		dae::TextComponent* m_pTextComponent{ nullptr };
	};
}