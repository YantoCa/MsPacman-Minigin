#include "ControllerBinding.h"

namespace dae {
	ControllerBinding::ControllerBinding(Gamepad& controller, Gamepad::ControllerButton button, std::unique_ptr<Command> command, KeyState state) noexcept
		: Binding(std::move(command), state), m_Controller{ controller }, m_Button{ button } { }

	void ControllerBinding::TryExecute() {
		bool shouldExecute = false;
		switch (m_Keystate)
		{
		case KeyState::OnHold:    
			shouldExecute = m_Controller.IsDown(m_Button); 
			break;
		case KeyState::OnPress:   
			shouldExecute = m_Controller.IsPressed(m_Button); 
			break;
		case KeyState::OnRelease: 
			shouldExecute = m_Controller.IsUp(m_Button); 
			break;
		}

		if (shouldExecute) m_pCommand->Execute();
	}
}