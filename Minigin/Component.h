#pragma once

namespace dae {
	class GameObject;

	class Component {
	public:
		explicit Component(GameObject* owner) : m_Owner(owner) {} // constructor
		virtual ~Component() = default;

		Component(const Component&) = delete;
		Component(Component&&) = delete;
		Component& operator=(const Component&) = delete;
		Component& operator=(Component&&) = delete;

		virtual void Update(float) {}
		virtual void Render() const {}
		virtual void FixedUpdate() {}

		GameObject* GetOwner() const { return m_Owner; }

	protected:
		GameObject* m_Owner;
	};
}