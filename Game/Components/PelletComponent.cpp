#include "PelletComponent.h"
#include "GameObject.h"
#include "RenderComponent.h" // To turn rendering off
#include "Events.h"

game::PelletComponent::PelletComponent(dae::GameObject& owner, int pointValue, bool isPowerPellet)
    : dae::Component(owner)
    , m_PointValue(pointValue)
    , m_IsPowerPellet(isPowerPellet)
{
}

void game::PelletComponent::Eat() {
    if (m_IsEaten) return;

    m_IsEaten = true;

    //off
    if (auto* pRender = GetOwner()->GetComponent<dae::RenderComponent>()) {
        //pRender->SetEnabled(false);
    }

    Notify(*GetOwner(), game::Event::PelletEaten);
}

void game::PelletComponent::Reset() {
    m_IsEaten = false;

    //on
    if (auto* pRender = GetOwner()->GetComponent<dae::RenderComponent>()) {
       //pRender->SetEnabled(true);
    }
}