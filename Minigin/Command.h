#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <memory>

namespace dae {
    class GameObject;

	class Command {
	public:
		virtual ~Command() = default;
		virtual void Execute() = 0;
	};

	// Iterations of Command
    
    class AddPointsCommand : public Command
    {
    public:
        AddPointsCommand(std::shared_ptr<GameObject> player, int points);

        void Execute() override;
    private:
        std::shared_ptr<GameObject> m_Player;
        int m_Points;
    };
}
