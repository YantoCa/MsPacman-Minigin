#pragma once
#include <glm/glm.hpp>

namespace dae
{
	class GameObject;

	class Transform final
	{
	public:
		explicit Transform(GameObject* owner = nullptr) : m_Owner(owner) {}

		const glm::vec3& GetLocalPosition() const;
		const glm::vec3& GetWorldPosition() const;

		void SetLocalPosition(float x, float y, float z = 0);
		void SetLocalPosition(const glm::vec3& position);

		void SetWorldPosition(float x, float y, float z = 0);
		void SetWorldPosition(const glm::vec3& position);

		void SetPositionDirty();
	private:
		GameObject* m_Owner;

		glm::vec3 m_LocalPosition{};
		glm::vec3 m_WorldPosition{};
		bool m_PositionIsDirty{ true };

		void UpdateWorldPosition();
	};
}