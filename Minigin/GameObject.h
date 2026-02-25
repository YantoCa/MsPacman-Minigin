#pragma once
#include <string>
#include <memory>
#include "Transform.h"

#include <vector>
#include "Component.h"


namespace dae
{
	class Texture2D;
	class GameObject final
	{
		Transform m_transform{};
		std::vector<std::unique_ptr<Component>> m_Components{}; 

	public:
		void Render() const;
		void Update(float deltaTime);
		void FixedUpdate();

		void SetTexture(const std::string& filename);
		void SetPosition(float x, float y);

		Transform& GetTransform() { return m_transform; };
		const Transform& GetTransform() const { return m_transform; };

		GameObject() = default;
		~GameObject() = default;
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

		// Component Functions
		template<typename T, typename... Args>
		T* AddComponent(Args&&... args) {

			static_assert(std::is_base_of<Component, T>::value, "T must be a Component");

			// Check if already exists
			if (T* exists = GetComponent<T>()) {
				return exists;
			}

			// Add to the vector
			auto component = std::make_unique<T>(this, std::forward<Args>(args)...);

			T* ptr = component.get();

			m_Components.push_back(std::move(component));
			return ptr;
		}

		template<typename T>
		T* GetComponent() const {
			for (const auto& component : m_Components) {
				if (T* correctComponent = dynamic_cast<T*>(component.get())) {
					return correctComponent;
				}
			}
			return nullptr; // Not found
		}

		template<typename T>
		void RemoveComponent() {
			m_Components.erase(
				std::remove_if(m_Components.begin(), m_Components.end(),
					[](const std::unique_ptr<Component>& component)
					{
						return dynamic_cast<T*>(component.get()) != nullptr;
					}),
				m_Components.end()
			);

		}

	};
}
