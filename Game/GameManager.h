#pragma once
#include "Component.h"
#include "Observer.h"
#include "Subject.h"
#include "MsPacmanEnums.h"
#include <string>
#include <vector>

namespace dae {
	class GameObject;
	class Scene; 
}

namespace game {
	class GridComponent;

	class GameManager : public dae::Observer, public dae::Component, public dae::Subject {
	public:
		explicit GameManager(dae::GameObject& owner, dae::Scene* currentScene);
		~GameManager() override = default;

		void Update(float) override;
		void LateUpdate(float) override;

		void OnNotify(const dae::GameObject& object, const std::string& event) override;

		GridComponent* GetMazeGrid() const; 
 
		void AddPlayer(dae::GameObject* pPlayer);
		void AddGhost(dae::GameObject* pGhost); // could attach player to it also making you control it?

		int GetMasterScore() const { return m_MasterScore; }
	private: 
		dae::Scene* m_ActiveScene{nullptr};

		std::vector<dae::GameObject*> m_Players{}; 
		std::vector<dae::GameObject*> m_Ghosts{}; 
		GridComponent* m_pMazeGrid{nullptr};

		std::vector<dae::GameObject*> m_Pellets{}; 
		std::vector<dae::GameObject*> m_Walls{}; 
		std::vector<dae::GameObject*> m_Fruit{}; 

		int m_MasterScore{0};
		int m_RemainingPellets{0};

		// Maze
		void LoadMaze(const Maze& newMaze); 
		void ClearUpMaze();
		void CreateMazeGameObjects(int totalRows, int totalCols);
		
		void ResetMaze(); // Players, ghost, pellets everything (when maze completed or started)
		
		// Players
		void ResetPlayers();
		void ResetGhosts();
	};
}