#pragma once
#include <glm/glm.hpp>
#include "Component.h"

namespace dae {
	class TransformComponent;

	class MovementComponent final : public Component { 
	public:
		MovementComponent(GameObject& owner, float speed = 100.f) noexcept;
		~MovementComponent() override = default;
		
		void Update(float deltaTime) override;

		void SetTargetDirection(const glm::vec3& direction);
		void SetSpeed(float speed);
	private:
		float m_Speed{};
		glm::vec3 m_Direction{}; 
	};
} 