#include "KeyboardBinding.h"

namespace dae {
	KeyboardBinding::KeyboardBinding(SDL_Scancode key, std::unique_ptr<Command> command, KeyState state) noexcept
		: Binding(std::move(command), state), m_Key{key} {}


	void KeyboardBinding::TryExecute(){
		int numKeys;
		const bool* keyboardState = (const bool*)SDL_GetKeyboardState(&numKeys);

		bool isDown = keyboardState[m_Key];
		bool shouldExecute = false;

		switch (m_Keystate)
		{
		case KeyState::OnHold:    
			shouldExecute = isDown; 
			break;
		case KeyState::OnPress:   
			shouldExecute = isDown && !m_PreviousState; 
			break;
		case KeyState::OnRelease: 
			shouldExecute = !isDown && m_PreviousState; 
			break;
		}

		if (shouldExecute) m_pCommand->Execute();
			m_PreviousState = isDown; 
	}
}