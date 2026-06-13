#pragma once
#include "Component.h"
#include <string>
#include "MsPacmanEnums.h"

namespace dae { 
    class GameObject;
}

namespace game
{

    class GhostComponent final : public dae::Component
    {
    public:
        explicit GhostComponent(dae::GameObject& owner, GhostType type, const std::string& normalTexture,
            dae::GameObject* pPacman, dae::GameObject* pBlinky = nullptr)
            : Component(owner), m_Type(type), m_NormalTexturePath(normalTexture)
            , m_pPacman(pPacman), m_pBlinky(pBlinky) {
        }

        ~GhostComponent() override = default;

        GhostType GetType() const { return m_Type; }
        const std::string& GetNormalTexturePath() const { return m_NormalTexturePath; }
        dae::GameObject* GetPacman() const { return m_pPacman; }
        dae::GameObject* GetBlinky() const { return m_pBlinky; }

        static const std::string& GetFrightenedTexturePath()
        {
            static const std::string path = "Characters/FrightenedGhost.png";
            return path;
        }

    private:
        GhostType m_Type;
        std::string m_NormalTexturePath;

        dae::GameObject* m_pPacman;
        dae::GameObject* m_pBlinky;
    };
}