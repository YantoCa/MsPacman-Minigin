#include "GameManager.h"
#include "GridComponent.h"
#include "GameObject.h"

#include "LevelLoader.h"
#include "Scene.h"

//
#include "GridMovementComponent.h"
#include "RenderComponent.h"

namespace game {
	GameManager::GameManager(dae::GameObject& owner) : dae::Component(owner) { }

	void GameManager::MazeTransition(const Maze& newMaze, dae::Scene& currentScene) {
		ClearUpMaze();

		std::string path;

		switch (newMaze)
		{
		case game::Maze::PinkMaze: path = "Data/Levels/Level1.csv";
			break;
		case game::Maze::LightBlueMaze: path = "Data/Levels/Level2.csv";
			break;
		case game::Maze::OrangeMaze:
			break;
		default:
			break;
		}
		auto matrix = LevelLoader::ParseCSV(path);
		if (matrix.empty()) return;

		int totalRows = static_cast<int>(matrix.size());
		int totalCols = static_cast<int>(matrix[0].size());

		// Create a gird gameobject
		auto gridObject = std::make_unique<dae::GameObject>();
		m_pMazeGrid = gridObject->AddComponent<GridComponent>(totalCols, totalRows, TILE_SIZE);

		for (int r = 0; r < totalRows; ++r) {
			for (int c = 0; c < totalCols; ++c) {
				m_pMazeGrid->SetTile(c, r, matrix[r][c]);
			}
		}
		currentScene.Add(std::move(gridObject));

		CreateMazeGameObjects(totalRows, totalCols, currentScene);

		ResetPlayers();
		ResetGhosts();
	}

	GridComponent* GameManager::GetMazeGrid() const { return m_pMazeGrid; }

	void GameManager::OnNotify(const dae::GameObject& object, dae::Event event) {
		object;
		event;
	}

	void GameManager::ResetMaze() {
		if (!m_pMazeGrid) return;

		for (auto* pellet : m_Pellets) {
			if (pellet) {
				// reset pellets

			}
		}
		for (auto* fruit : m_Fruit) {
			if (fruit) {
				// reset pellets
			}
		}

		ResetPlayers();
		ResetGhosts();
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
				pGridMove->SetGrid(m_pMazeGrid); // update maze
				pGridMove->SnapToGrid();
			}
		}
	}
	void GameManager::ResetGhosts() {
		if (!m_pMazeGrid) return;

		// Snap players back to their starting positions
		auto& ghostpawns = m_pMazeGrid->GetGhostSpawnPoints();
		for (size_t i = 0; i < m_Players.size(); ++i)
		{
			if (!m_Players[i]) continue;
			glm::vec3 spawnPos = (i < ghostpawns.size()) ? ghostpawns[i] : glm::vec3{ 0.0f };
			m_Players[i]->GetTransform().SetWorldPosition(spawnPos);

			if (auto* pGridMove = m_Players[i]->GetComponent<GridMovementComponent>())
			{
				pGridMove->SetGrid(m_pMazeGrid); // update maze
				pGridMove->SnapToGrid();
			}
		}
	}

	void GameManager::AddPlayer(dae::GameObject* pPlayer) {
		if (!pPlayer) return;

		m_Players.push_back(pPlayer);

		ResetPlayers();
	}
	void GameManager::AddGhost(dae::GameObject* pGhost) {
		if (!pGhost) return;

		m_Ghosts.push_back(pGhost);

		ResetGhosts();
	}


	// Maze creation helper functions
	void GameManager::ClearUpMaze() {
		for (auto* pellet : m_Pellets) { if (pellet) pellet->Destroy(); }
		for (auto* wall : m_Walls) { if (wall)    wall->Destroy(); }
		for (auto* ghost : m_Ghosts) { if (ghost)   ghost->Destroy(); }
		for (auto* fruit : m_Fruit) { if (fruit)   fruit->Destroy(); }

		m_Pellets.clear();
		m_Walls.clear();
		m_Ghosts.clear();
		m_Fruit.clear();

		if (m_pMazeGrid) {
			m_pMazeGrid->GetOwner()->Destroy();
			m_pMazeGrid = nullptr;
		}
	}

	void GameManager::CreateMazeGameObjects(int totalRows, int totalCols, dae::Scene& currentScene) {
		if (!m_pMazeGrid) return;

		for (int r = 0; r < totalRows; ++r) {
			for (int c = 0; c < totalCols; ++c) {
				TileType tileId = m_pMazeGrid->GetTileType(c, r);
				glm::vec3 centerPos = m_pMazeGrid->GridToWorldCenter(c, r);

				switch (tileId)
				{
				case TileType::Wall:
				{
					auto wall = std::make_unique<dae::GameObject>();
					wall->GetTransform().SetWorldPosition(centerPos);
					wall->AddComponent<dae::RenderComponent>("Tiles/Wall.png");
					m_Walls.push_back(wall.get()); // Tracked!
					currentScene.Add(std::move(wall));
				}
				break;

				case TileType::Pellet:
				{
					auto dot = std::make_unique<dae::GameObject>();
					dot->GetTransform().SetWorldPosition(centerPos);
					dot->AddComponent<dae::RenderComponent>("Tiles/Pellet.png");
					m_Pellets.push_back(dot.get()); // Tracked!
					currentScene.Add(std::move(dot));
				}
				break;

				case TileType::PowerPellet:
				{
					auto powerPellet = std::make_unique<dae::GameObject>();
					powerPellet->GetTransform().SetWorldPosition(centerPos);
					powerPellet->AddComponent<dae::RenderComponent>("Tiles/PowerPellet.png");
					m_Pellets.push_back(powerPellet.get()); // Tracked under pellet array
					currentScene.Add(std::move(powerPellet));
				}
				break;

				case TileType::Player1Spawn:
				case TileType::Player2Spawn:
					m_pMazeGrid->AddPlayerSpawnPoint(centerPos);
					break;

				case TileType::GhostSpawn:
					m_pMazeGrid->AddGhostSpawnPoint(centerPos);
					break;
				}
			}
		}
	}
}