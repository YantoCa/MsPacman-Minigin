#pragma once
#include <glm/glm.hpp>
#include "Component.h"

namespace dae
{
	class GameObject;

	class TransformComponent final : public Component
	{
	public:
		TransformComponent(GameObject* owner, glm::vec3 const& position = { 0, 0, 0 });
		~TransformComponent() override = default;

		const glm::vec3& GetLocalPosition() const;
		const glm::vec3& GetWorldPosition();

		void SetLocalPosition(float x, float y, float z = 0);
		void SetLocalPosition(const glm::vec3& position);

		void SetWorldPosition(float x, float y, float z = 0);
		void SetWorldPosition(const glm::vec3& position);

		void SetPositionDirty();
	private: 
		glm::vec3 m_LocalPosition{};
		glm::vec3 m_WorldPosition{};
		bool m_PositionIsDirty{ true };

		void UpdateWorldPosition();
	};
}