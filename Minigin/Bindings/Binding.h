#pragma once
#include <memory>
#include "Command.h"

namespace dae {
	enum class KeyState {
		OnPress,
		OnHold,
		OnRelease
	};

	class Binding {
	public:
		Binding(std::unique_ptr<Command> command, KeyState state) noexcept
			: m_pCommand{std::move(command)}, m_Keystate{ state } {	}
		virtual ~Binding() noexcept = default;

		virtual void TryExecute() = 0;
	protected:
		std::unique_ptr<Command> m_pCommand;
		KeyState m_Keystate;
	};
}