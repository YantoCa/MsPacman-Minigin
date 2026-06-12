#pragma once
#include "Component.h"
#include "Observer.h"
#include "MsPacmanEnums.h"

namespace dae {
	class GameObject;
	class Scene; 
}

namespace game {
	class GridComponent;

	class GameManager : public dae::Observer, public dae::Component {
	public:
		explicit GameManager(dae::GameObject& owner);
		~GameManager() override = default;

		void MazeTransition(const Maze& newMaze, dae::Scene& currentScene);// completly switch to a new maze
		GridComponent* GetMazeGrid() const; 

		void OnNotify(const dae::GameObject& object, dae::Event event) override;
 
		void ResetMaze(); // Players, ghost, pellets everything (when maze completed or started)
		void ResetPlayers();
		void AddPlayer(dae::GameObject* pPlayer);

	private: 
		GridComponent* m_pMazeGrid{nullptr};
		std::vector<dae::GameObject*> m_Players{}; 
		std::vector<dae::GameObject*> m_Pellets{}; 
		std::vector<dae::GameObject*> m_Ghosts{}; 
		std::vector<dae::GameObject*> m_Fruit{}; 
		
		// UI seperate from grid, but listens to notifys
		//int m_Highscore{};
	};
}