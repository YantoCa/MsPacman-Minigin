#pragma once
#include "Binding.h"
#include "Gamepad.h"

namespace dae {
	class ControllerBinding final : public Binding {
	public:
		ControllerBinding(Gamepad& controller, Gamepad::ControllerButton m_Button, std::unique_ptr<Command> command, KeyState state) noexcept;

		void TryExecute() override;

	private:
		Gamepad& m_Controller;
		Gamepad::ControllerButton m_Button;
	};
}