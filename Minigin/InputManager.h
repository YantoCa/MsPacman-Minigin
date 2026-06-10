#pragma once
#include "Singleton.h"
#include "Binding.h"
#include "Gamepad.h"

namespace dae
{
	class InputManager final : public Singleton<InputManager>
	{
	public:
		bool ProcessInput();

		Gamepad& AddController(unsigned int id);
		//void DeleteController(unsigned int id);
		//Gamepad& GetController(int controllerIndex);

		void AddBinding(std::unique_ptr<Binding> binding);
		void ClearBinding();
	private: 
		std::vector<std::unique_ptr<Binding>> m_Bindings; 
		std::vector<std::unique_ptr<Gamepad>> m_Controllers; 
	};

}
