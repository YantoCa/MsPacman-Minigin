#include "GameObject.h"
#include "Components/Component.h"
#include "Components/RenderComponent.h"

#include <stdexcept> // TODO for std::outofrange

namespace dae {
    GameObject::GameObject() {
        AddComponent<TransformComponent>();
    }

    GameObject::~GameObject() {
        for (auto* child : m_Childeren) {
            if(child)
                child->m_Parent = nullptr;
        }

        m_Childeren.clear();
        if (m_Parent) {
            m_Parent->RemoveChild(this);
            m_Parent = nullptr;
        }
    }

    void GameObject::Update(float deltaTime)
    {
        for (const auto& component : m_Components)
        {
            component->Update(deltaTime);
        }
    }

    void GameObject::LateUpdate(float deltaTime) {
        for (const auto& component : m_Components)
        {
            component->LateUpdate(deltaTime);
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

    TransformComponent& GameObject::GetTransform()
    {
        return *GetComponent<TransformComponent>();
    }

    const TransformComponent& GameObject::GetTransform() const
    {
        return *GetComponent<TransformComponent>();
    }

    void GameObject::SetPosition(float x, float y, float z) {
        GetTransform().SetLocalPosition({x,y, z});
    }

    GameObject* GameObject::GetParent() const { return m_Parent; }


    void GameObject::SetParent(GameObject* parent, bool keepWorldPosition)
    {
        if (parent == this || m_Parent == parent || IsChildOf(parent))
            return;

        TransformComponent& transform = GetTransform();
        glm::vec3 worldPos = transform.GetWorldPosition();

        // Remove from previous parent
        if (m_Parent)
        {
            m_Parent->RemoveChild(this);
        }

        m_Parent = parent;

        if (m_Parent)
        {
            m_Parent->AddChild(this);

            if (keepWorldPosition)
            {
                glm::vec3 parentWorld = m_Parent->GetTransform().GetWorldPosition();
                transform.SetLocalPosition(worldPos - parentWorld);
            }
        }
        else
        {
            transform.SetLocalPosition(worldPos);
        }

        transform.SetPositionDirty();
    }

    void GameObject::AddChild(GameObject* child)
    {
        if (!child) return;
         
        if (std::find(m_Childeren.begin(), m_Childeren.end(), child) == m_Childeren.end())
        {
            m_Childeren.push_back(child);
        }
    }

    void GameObject::RemoveChild(GameObject* child)
    {
        if (!child) return;
        m_Childeren.erase(std::remove(m_Childeren.begin(), m_Childeren.end(), child), m_Childeren.end());
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

    void GameObject::MarkForDeletion(){
        if (m_isMarkedForDeletion) return;

        m_isMarkedForDeletion = true;  // Mark itself for destruction

        if (m_Parent)
        {
            m_Parent->RemoveChild(this);
            m_Parent = nullptr;
        }

        for (auto* child : m_Childeren) {
            child->MarkForDeletion(); // and its children	
        }
    }; 	
}