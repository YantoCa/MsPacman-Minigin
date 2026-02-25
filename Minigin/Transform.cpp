#include "Transform.h"
#include "GameObject.h"

namespace dae {
	void Transform::SetPositionDirty() { m_PositionIsDirty = true; }
	const glm::vec3& Transform::GetLocalPosition() const { return m_LocalPosition; }
	const glm::vec3& Transform::GetWorldPosition() const { 
		if (m_PositionIsDirty)
			const_cast<Transform*>(this)->UpdateWorldPosition();

		return m_WorldPosition;
	}

	void Transform::SetLocalPosition(float x, float y, float z) {
		SetLocalPosition(glm::vec3( x,y,z ));
	}
	void Transform::SetLocalPosition(const glm::vec3& position) {
		if (m_LocalPosition != position)
		{
			m_LocalPosition = position;
			SetPositionDirty();
		}
	}

	void Transform::SetWorldPosition(float x, float y, float z) {
		SetWorldPosition(glm::vec3(x, y, z));
	}
	void Transform::SetWorldPosition(const glm::vec3& position) {
		if (m_Owner && m_Owner->GetParent())
		{
			glm::vec3 parentWorldPos = m_Owner->GetParent()->GetTransform().GetWorldPosition(); 
			m_LocalPosition = position - parentWorldPos;
		}
		else
		{
			m_LocalPosition = position;
		}

		SetPositionDirty();
	}

	void Transform::UpdateWorldPosition() {
		if (m_PositionIsDirty)
		{
			if (m_Owner && m_Owner->GetParent())
				m_WorldPosition = m_Owner->GetParent()->GetTransform().GetWorldPosition() + m_LocalPosition;
			else
				m_WorldPosition = m_LocalPosition;

			m_PositionIsDirty = false;
		}
	}
}