#include "GameManager.h"
#include "GridComponent.h"
#include "GameObject.h"

#include "LevelLoader.h"


//
#include "GridMovementComponent.h"

namespace game {
	GameManager::GameManager(dae::GameObject& owner) : dae::Component(owner) { }

	void GameManager::MazeTransition(const Maze& newMaze, dae::Scene& currentScene) {
		switch (newMaze)
		{
		case game::Maze::PinkMaze:
		m_pMazeGrid = LevelLoader::LoadLevel("Data/Levels/Level1.csv", currentScene);
			break;
		case game::Maze::LightBlueMaze:
			break;
		case game::Maze::OrangeMaze:
			break;
		default:
			break;
		}
	}

	GridComponent* GameManager::GetMazeGrid() const { return m_pMazeGrid; }

	void GameManager::OnNotify(const dae::GameObject& object, dae::Event event) {
		object;
		event;
	}

	void GameManager::ResetMaze() {
		// what owns.... the maze components now.
	}

	void GameManager::ResetPlayers() {
		if (!m_pMazeGrid) return;

		// Snap players back to their starting positions
		auto& playerSpawns = m_pMazeGrid->GetPlayerSpawnPoints(); 
		for (size_t i = 0; i < m_Players.size(); ++i)
		{
			if (!m_Players[i]) continue;
			glm::vec3 spawnPos = (i < playerSpawns.size()) ? playerSpawns[i] : glm::vec3{ 0.0f };
			m_Players[i]->GetTransform().SetWorldPosition(spawnPos);
			if (auto* pGridMove = m_Players[i]->GetComponent<GridMovementComponent>())
			{
				pGridMove->SnapToGrid();
			}
		}
	}

	void GameManager::AddPlayer(dae::GameObject* pPlayer) {
		if (!pPlayer) return;

		m_Players.push_back(pPlayer);
	}

}