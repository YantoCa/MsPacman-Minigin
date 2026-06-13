#pragma once
#include "Component.h"
#include <memory>

namespace dae
{
	class State;

	class FSMComponent final : public Component
	{
	public:
		explicit FSMComponent(GameObject& owner);
		virtual ~FSMComponent() = default;

		void Update(float deltaTime) override;
		 
		void ChangeState(std::unique_ptr<State> pNewState);
		State* GetCurrentState() const { return m_pCurrentState.get(); }

		FSMComponent(const FSMComponent& other) = delete;
		FSMComponent(FSMComponent&& other) = delete;
		FSMComponent& operator=(const FSMComponent& other) = delete;
		FSMComponent& operator=(FSMComponent&& other) = delete;

	private:
		std::unique_ptr<State> m_pCurrentState{ nullptr };
	};
}