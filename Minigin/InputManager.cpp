#include <SDL3/SDL.h>
#include "InputManager.h"

#include <SDL3/SDL.h>
#include <backends/imgui_impl_sdl3.h>
#include "InputManager.h"

bool dae::InputManager::ProcessInput()
{
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_EVENT_QUIT) {
			return false;
		}
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
