#include "FSMComponent.h"
#include "State.h"
#include <utility>

namespace dae {
	FSMComponent::FSMComponent(GameObject& owner)
		: Component(owner)
	{
	}

	void FSMComponent::Update(float deltaTime)
	{
		if (m_pCurrentState)
		{
			m_pCurrentState->Update(deltaTime);
		}
	}

	void FSMComponent::ChangeState(std::unique_ptr<State> pNewState)
	{
		if (m_pCurrentState)
		{
			m_pCurrentState->OnExit();
		}

		m_pCurrentState = std::move(pNewState);

		if (m_pCurrentState)
		{
			m_pCurrentState->OnEnter();
		}
	}
}