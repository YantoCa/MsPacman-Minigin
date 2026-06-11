#include "MovementComponent.h" 
#include "GameObject.h"

namespace game {
	MovementComponent::MovementComponent(dae::GameObject& owner, float speed) noexcept
		: Component(owner) {
		m_Speed = speed;
	}

	void MovementComponent::Update(float deltaTime) {
		if (m_Direction == glm::vec3{ 0.0f, 0.0f, 0.0f })
			return;
		 
		auto& transform = GetOwner()->GetTransform();
		glm::vec3 currentPosition = transform.GetLocalPosition();
		 
		glm::vec3 velocity = m_Direction;

		if (glm::dot(velocity, velocity) > 0.0f)
		{
			velocity = glm::normalize(velocity);
		}

		currentPosition += velocity * m_Speed * deltaTime;
		 
		transform.SetLocalPosition(currentPosition);

		m_Direction = glm::vec3{ 0.0f, 0.0f, 0.0f }; // Reset so it doesnt move infinitely // TODO Delete maybe for arcadic feel later on.
	}


	void MovementComponent::SetTargetDirection(const glm::vec3& direction) { m_Direction = direction; }
	void MovementComponent::SetSpeed(float speed) { m_Speed = speed; }
}