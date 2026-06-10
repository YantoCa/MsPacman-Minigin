#include "TransformComponent.h"
#include "GameObject.h"

namespace dae {
	TransformComponent::TransformComponent(GameObject& owner, glm::vec3 const& position)
		: Component(owner), m_LocalPosition{ position }, m_WorldPosition{}
	{ }

	void TransformComponent::SetPositionDirty() {
		if (m_PositionIsDirty) return;

		m_PositionIsDirty = true;

		if (GetOwner()) {
			for (auto& child : GetOwner()->GetChildren()) {
				if (child != nullptr) {
					child->GetTransform().SetPositionDirty();
				}
			}
		}
	}
	const glm::vec3& TransformComponent::GetLocalPosition() const { return m_LocalPosition; }
	const glm::vec3& TransformComponent::GetWorldPosition() {
		if (m_PositionIsDirty)
			 UpdateWorldPosition();

		return m_WorldPosition;
	}

	void TransformComponent::SetLocalPosition(float x, float y, float z) {
		SetLocalPosition(glm::vec3( x,y,z ));
	}
	void TransformComponent::SetLocalPosition(const glm::vec3& position) {
		if (m_LocalPosition != position)
		{
			m_LocalPosition = position;
			SetPositionDirty();
		}
	}

	void TransformComponent::SetWorldPosition(float x, float y, float z) {
		SetWorldPosition(glm::vec3(x, y, z));
	}
	void TransformComponent::SetWorldPosition(const glm::vec3& position) {
		if (GetOwner() && GetOwner()->GetParent())
		{
			glm::vec3 parentWorldPos = GetOwner()->GetParent()->GetTransform().GetWorldPosition();
			m_LocalPosition = position - parentWorldPos;
		}
		else
		{
			m_LocalPosition = position;
		}

		SetPositionDirty();
	}

	void TransformComponent::UpdateWorldPosition() {
		if (m_PositionIsDirty)
		{
			if (GetOwner() && GetOwner()->GetParent())
				m_WorldPosition = GetOwner()->GetParent()->GetTransform().GetWorldPosition() + m_LocalPosition;
			else
				m_WorldPosition = m_LocalPosition;

			m_PositionIsDirty = false;
		}
	}
}