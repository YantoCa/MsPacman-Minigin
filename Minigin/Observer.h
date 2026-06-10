#pragma once
#include <vector>

namespace dae {
	
	class GameObject;
	enum class Event;

	class Observer {
	public:
		virtual ~Observer() = default;
		virtual void OnNotify(const GameObject& object, Event event) = 0;
	};
}