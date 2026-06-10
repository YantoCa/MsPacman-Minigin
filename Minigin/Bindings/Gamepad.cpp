#include "Gamepad.h"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <Xinput.h>
#pragma comment(lib, "xinput.lib")
#endif

namespace dae
{
	class Gamepad::GamepadImpl
	{
		unsigned int m_ControllerIndex;
#ifdef _WIN32
		XINPUT_STATE m_PreviousState{};
		XINPUT_STATE m_CurrentState{};
		WORD m_ButtonsChangesThisFrame{};
#endif

	public:
		GamepadImpl(unsigned int controllerIndex) : m_ControllerIndex{ controllerIndex }
		{
#ifdef _WIN32
			ZeroMemory(&m_PreviousState, sizeof(XINPUT_STATE));
			ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));
#endif
		}

		void Update()
		{
#ifdef _WIN32
			m_PreviousState = m_CurrentState;
			ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));
			XInputGetState(m_ControllerIndex, &m_CurrentState);

			auto buttonChanges = m_CurrentState.Gamepad.wButtons ^ m_PreviousState.Gamepad.wButtons;
			m_ButtonsChangesThisFrame = static_cast<WORD>(buttonChanges);
#endif
		}

		bool IsDown([[maybe_unused]] ControllerButton button) const
		{
#ifdef _WIN32
			return m_CurrentState.Gamepad.wButtons & static_cast<WORD>(button);
#else
			return false; // Emscripten/Linux safe fallback
#endif
		}

		bool IsUp([[maybe_unused]] ControllerButton button) const
		{
#ifdef _WIN32
			return (m_ButtonsChangesThisFrame & static_cast<WORD>(button)) && !(m_CurrentState.Gamepad.wButtons & static_cast<WORD>(button));
#else
			return false;
#endif
		}

		bool IsPressed([[maybe_unused]] ControllerButton button) const
		{
#ifdef _WIN32
			return (m_ButtonsChangesThisFrame & static_cast<WORD>(button)) && (m_CurrentState.Gamepad.wButtons & static_cast<WORD>(button));
#else
			return false;
#endif
		}

		unsigned int GetIndex() const { return m_ControllerIndex; }
	};

	Gamepad::Gamepad(unsigned int controllerIndex) : m_pImpl{ std::make_unique<GamepadImpl>(controllerIndex) } {}
	Gamepad::~Gamepad() = default;
	void Gamepad::Update() { m_pImpl->Update(); }
	bool Gamepad::IsDown(ControllerButton button) const { return m_pImpl->IsDown(button); }
	bool Gamepad::IsUp(ControllerButton button) const { return m_pImpl->IsUp(button); }
	bool Gamepad::IsPressed(ControllerButton button) const { return m_pImpl->IsPressed(button); }
	unsigned int Gamepad::GetIndex() const { return m_pImpl->GetIndex(); }
}