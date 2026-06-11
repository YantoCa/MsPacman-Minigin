#pragma once
#include "Component.h"
#include "TextComponent.h"

namespace game {
	class FPSComponent final : public dae::Component {
	public:
		FPSComponent(dae::GameObject& owner, std::shared_ptr<dae::Font> font);

		void Update(float deltaTime) override;
	private:
		dae::TextComponent* m_TextComponent{};

		float m_AccumulatedTime{ 0.f };
	};
}