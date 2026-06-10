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

    //// Bind points to keyboard currently and later on movement with a seperate component
    // 
    //class MoveCommand final : public Command {
    //public:
    //    MoveCommand(GameObject& object, const  glm::vec2& direction);

    //    void Execute() override;

    //private: 
    //    GameObject& m_Object;

    //    glm::vec2 m_Direction;
    //};
    //

    // delete seperate
    class AddPointsCommand final : public Command
    {
    public:
        AddPointsCommand(GameObject* player, int points);

        void Execute() override;
    private:
        GameObject* m_Player;
        int m_Points;
    };
}
