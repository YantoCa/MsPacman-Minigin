#pragma once
#include "Component.h"
#include "Observer.h"
#include "MsPacmanEnums.h"
#include <string>
#include <vector>

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

		void OnNotify(const dae::GameObject& object, const std::string& event) override;
 
		void AddPlayer(dae::GameObject* pPlayer);
		void AddGhost(dae::GameObject* pGhost); // could attach player to it also making you control it?
	private: 
		std::vector<dae::GameObject*> m_Players{}; 
		std::vector<dae::GameObject*> m_Ghosts{}; 
		GridComponent* m_pMazeGrid{nullptr};

		std::vector<dae::GameObject*> m_Pellets{}; 
		std::vector<dae::GameObject*> m_Walls{}; 
		std::vector<dae::GameObject*> m_Fruit{}; 

		// Maze
		void ClearUpMaze();
		void CreateMazeGameObjects(int totalRows, int totalCols, dae::Scene& currentScene);
		
		void ResetMaze(); // Players, ghost, pellets everything (when maze completed or started)
		
		// Players
		void ResetPlayers();
		void ResetGhosts();
		// UI seperate from grid, but listens to notifys
		//int m_Highscore{};
	};
}