#pragma once
#include <string>

namespace dae {
	
	class GameObject;

	class Observer {
	public:
		virtual ~Observer() = default;
		virtual void OnNotify(const GameObject& object, const std::string& event) = 0;
	};
}