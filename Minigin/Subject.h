#pragma once
#include <vector>

namespace dae {
	class Observer;
	class GameObject;
	enum class Event;

	class Subject {
	public:
		Subject() = default;
		virtual ~Subject() = default;

		void AddObserver(Observer* observer);
		void RemoveObserver(Observer* observer);

	protected:
		void Notify(const GameObject& object, Event event);
	private:
		std::vector<Observer*> m_Observers{};
	};
}