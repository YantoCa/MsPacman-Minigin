#pragma once
#include "Singleton.h"
#include "Binding.h"

namespace dae
{
	class InputManager final : public Singleton<InputManager>
	{
	public:
		bool ProcessInput();

		void AddBinding(std::unique_ptr<Binding> binding);
		void ClearBinding();

	private: 
		std::vector<std::unique_ptr<Binding>> m_Bindings; 
	};

}
