#pragma once
#include <vector>
#include <string>

namespace dae {
	class Observer;
	class GameObject;

	class Subject {
	public:
		Subject() = default;
		virtual ~Subject() = default;

		void AddObserver(Observer* observer);
		void RemoveObserver(Observer* observer);

	protected:
		void Notify(const GameObject& object, const std::string& event);
	private:
		std::vector<Observer*> m_Observers{};
	};
}