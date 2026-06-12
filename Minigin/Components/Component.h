#pragma once

namespace dae {
	class GameObject;

	class Component {
	public:
		virtual ~Component() = default;

		Component(const Component&) = delete;
		Component(Component&&) = delete;
		Component& operator=(const Component&) = delete;
		Component& operator=(Component&&) = delete;

		virtual void Update(float) {}
		virtual void LateUpdate(float) {}
		virtual void Render() const {}
		virtual void FixedUpdate() {}

		void SetActive(bool isActive) { m_isActive = isActive; }
		bool IsActive() const { return m_isActive; }

		GameObject* GetOwner() const { return m_pOwner; }
	protected:
		explicit Component(GameObject& owner) : m_pOwner(&owner) {}
	private:
		GameObject* m_pOwner;
		bool m_isActive{ true };
	};
}