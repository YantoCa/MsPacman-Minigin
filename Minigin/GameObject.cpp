#include "GameObject.h"
#include "Component.h"
#include "RenderComponent.h"

#include <stdexcept> // TODO for std::outofrange

namespace dae {
    void GameObject::Update(float deltaTime)
    {
        for (const auto& component : m_Components)
        {
            component->Update(deltaTime);
        }
    }

    void GameObject::FixedUpdate()
    {
        for (const auto& component : m_Components)
        {
            component->FixedUpdate();
        }
    }

    void GameObject::Render() const
    {
        for (const auto& component : m_Components)
        {
            component->Render();
        }
    }

    void GameObject::SetPosition(float x, float y, float z) {
        m_transform.SetLocalPosition({ x,y, z });
    }

    GameObject::~GameObject() {
        for (auto* child : m_Childeren) {
            child->m_Parent = nullptr;
        }

        m_Childeren.clear();
        SetParent(nullptr, false);
    }

    GameObject* GameObject::GetParent() const { return m_Parent; }


    void GameObject::SetParent(GameObject* parent, bool keepWorldPosition)
    {
        if (parent == this || m_Parent == parent || IsChildOf(parent))
            return;

        glm::vec3 worldPos = m_transform.GetWorldPosition();

        // Remove from previous parent
        if (m_Parent)
        {
            auto& siblings = m_Parent->m_Childeren;
            siblings.erase(std::remove(siblings.begin(), siblings.end(), this), siblings.end());
        }

        m_Parent = parent;

        if (m_Parent)
        {
            m_Parent->m_Childeren.push_back(this);

            if (keepWorldPosition)
            {
                glm::vec3 parentWorld = m_Parent->GetTransform().GetWorldPosition();
                m_transform.SetLocalPosition(worldPos - parentWorld);
            }
        }
        else
        {
            m_transform.SetLocalPosition(worldPos);
        }

        m_transform.SetPositionDirty();
    }

    size_t GameObject::GetChildCount() const { return m_Childeren.size(); };

    GameObject* GameObject::GetChildAt(size_t index) const {
        if (index >= m_Childeren.size()) throw std::out_of_range("Child index out of bounds"); 
        return m_Childeren[index];
    }

    const std::vector<GameObject*>& GameObject::GetChildren() const { return m_Childeren; }

    bool GameObject::IsChildOf(const GameObject* potentialChild) const {
        if (m_Parent == nullptr) return false;
        for (const GameObject* parent = m_Parent; parent != nullptr; parent = parent->m_Parent)
        {
            if (parent == potentialChild) return true;
        }
        return false;
    }
}