#include "GameObject.h"
#include "Component.h"
#include "RenderComponent.h"

void dae::GameObject::Update(float deltaTime)
{
    for (const auto& component : m_Components)
    {
        component->Update(deltaTime);
    }
}

void dae::GameObject::FixedUpdate()
{
    for (const auto& component : m_Components)
    {
        component->FixedUpdate();
    }
}

void dae::GameObject::Render() const
{
    for (const auto& component : m_Components)
    {
        component->Render();
    }
}

void dae::GameObject::SetPosition(float x, float y) {
    m_transform.SetPosition({ x,y,0 });
}