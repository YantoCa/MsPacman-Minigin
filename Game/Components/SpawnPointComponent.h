#pragma once
#include "Component.h"

namespace game {
	class SpawnPointComponent final : public dae::Component {
	public:
		explicit SpawnPointComponent(dae::GameObject& owner);
		virtual ~SpawnPointComponent() override = default;
	}; 
}