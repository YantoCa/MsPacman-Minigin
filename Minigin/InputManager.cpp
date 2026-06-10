#include <SDL3/SDL.h>
#include <backends/imgui_impl_sdl3.h>
#include "InputManager.h" 
#include <iostream>

bool dae::InputManager::ProcessInput()
{
	for (auto& controller : m_Controllers)
	{
		controller->Update();
	}

	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_EVENT_QUIT) {
			return false;
		}

		//process event for IMGUI
		ImGui_ImplSDL3_ProcessEvent(&e); // TODO bind to bindings?
	}

	for (auto& binding : m_Bindings) {
		binding->TryExecute();
	}

	return true;
}

void dae::InputManager::AddBinding(std::unique_ptr<Binding> binding) {
	m_Bindings.emplace_back(std::move(binding));
}

void dae::InputManager::ClearBinding() {
	m_Bindings.clear();
}


dae::Gamepad& dae::InputManager::AddController(unsigned int id) {
	if (m_Controllers.size() >= 4) { // Magic number is controlled by the limit of XInput
		std::cout << "Controller limit reached.\n";
	}

	for (auto& controller : m_Controllers)
	{
		// Controller already exists
		if (controller && controller->GetIndex() == id)
			return *controller;
	}

	m_Controllers.emplace_back(std::make_unique<Gamepad>(id));
	std::cout << "Controller creaded with id: " << static_cast<int>(id) << "\n";

	return *m_Controllers.back();
}

//void dae::InputManager::DeleteController(unsigned int id) {
//	for (auto& controller : m_Controllers)
//	{ 
//		if (controller && controller->GetIndex() == id) {
//
//			std::cout << "Controller DELETED with id: " << static_cast<int>(id) << "\n";
//		}
//	}
//
//	std::cout << "Controller with id: " << static_cast<int>(id) << ", Not Found.\n";
//}

//dae::Gamepad& dae::InputManager::GetController(int listIndex) {
//	assert(controllerIndex <= m_Controllers.size());
//
//	return *m_Controllers.at(controllerIndex);
//}

