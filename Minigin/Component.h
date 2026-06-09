#pragma once

namespace dae {
	class GameObject;

	class Component {
	public:
		explicit Component(GameObject* pOwner) : m_pOwner(pOwner) {} 
		virtual ~Component() = default;

		Component(const Component&) = delete;
		Component(Component&&) = delete;
		Component& operator=(const Component&) = delete;
		Component& operator=(Component&&) = delete;

		virtual void Update(float) {}
		virtual void Render() const {}
		virtual void FixedUpdate() {}

	protected:
		GameObject* GetOwner() const { return m_pOwner; }
	private:
		GameObject* m_pOwner;
	};
}