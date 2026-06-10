#pragma once
#include "Binding.h"
#include <SDL3/SDL.h>

namespace dae {
	class KeyboardBinding final : public Binding {
	public:
		KeyboardBinding(SDL_Scancode key, std::unique_ptr<Command> command, KeyState state) noexcept;

		void TryExecute() override;

	private:
		SDL_Scancode m_Key;
		bool m_PreviousState{ false };
	};
}