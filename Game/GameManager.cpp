#include "GameManager.h"
#include "GridComponent.h"
#include "GameObject.h"

#include "LevelLoader.h"
#include "Scene.h"
#include "Events.h"
#include "ServiceLocator.h"

//
#include "GridMovementComponent.h"
#include "RenderComponent.h"
#include "PelletComponent.h"
#include "BoxColliderComponent.h"
#include "PointsComponent.h"
#include <iostream>

namespace game {
	GameManager::GameManager(dae::GameObject& owner, dae::Scene* scene)
		: dae::Component(owner), m_pActiveScene{ scene } {
		LoadMaze(game::Maze::PinkMaze); // load the starting maze
	}

	void GameManager::Update(float) {

	}

	// Colliders
	void GameManager::LateUpdate(float) { // happens after Movement has been updated
		for (auto* player : m_Players)
		{
			if (!player || player->IsMarkedForDeletion()) continue; 

			auto* playerCollider = player->GetComponent<dae::BoxColliderComponent>();
			if (!playerCollider) continue;  
			 
			// Collision Pellets
			for (auto* pellet : m_Pellets)
			{
				if (!pellet) continue;

				auto* pPelletComp = pellet->GetComponent<game::PelletComponent>();
				 
				if (!pPelletComp || pPelletComp->IsEaten()) continue;

				auto* pelletCollider = pellet->GetComponent<dae::BoxColliderComponent>();
				if (!pelletCollider) continue;
				 
				if (playerCollider->IsOverlapping(*pelletCollider))
				{ 
					pPelletComp->Eat();
				}
			}
		}
	}

	void GameManager::LoadMaze(const Maze& newMaze) {
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
		gridObject->GetTransform().SetWorldPosition(GetOwner()->GetTransform().GetWorldPosition());
		m_pMazeGrid = gridObject->AddComponent<GridComponent>(totalCols, totalRows, TILE_SIZE);

		for (int r = 0; r < totalRows; ++r) {
			for (int c = 0; c < totalCols; ++c) {
				m_pMazeGrid->SetTile(c, r, matrix[r][c]);
			}
		}

		m_pActiveScene->Add(std::move(gridObject));

		CreateMazeGameObjects(totalRows, totalCols);

		ResetPlayers();
		ResetGhosts();
	}

	GridComponent* GameManager::GetMazeGrid() const { return m_pMazeGrid; }

	void GameManager::OnNotify(const dae::GameObject& object, const std::string& event) {
		// Pellet eaten event
		if (event == game::Event::PelletEaten) {
			auto* pPellet = object.GetComponent<game::PelletComponent>();
			if (!pPellet) return;

			dae::ServiceLocator::GetSoundSystem().PlaySound(0, 1.f);

			// 3. Update the global game state state
			m_MasterScore += pPellet->GetPointValue();
			m_RemainingPellets--;

			// UI Notifys
			Notify(*GetOwner(), "ScoreChanged");
			 
			if (m_RemainingPellets <= 0)
			{
				NextLevelLogic();
				return;
			}

			/*if (pPellet->IsPowerPellet())
			{
				TriggerGhostFrightenedMode();
			}*/
		}
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
		 
		auto& ghostpawns = m_pMazeGrid->GetGhostSpawnPoints();
		for (size_t i = 0; i < m_Ghosts.size(); ++i)
		{
			if (!m_Ghosts[i]) continue;
			glm::vec3 spawnPos = (i < ghostpawns.size()) ? ghostpawns[i] : glm::vec3{ 0.0f };
			m_Ghosts[i]->GetTransform().SetWorldPosition(spawnPos);

			if (auto* pGridMove = m_Ghosts[i]->GetComponent<GridMovementComponent>())
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

	void GameManager::CreateMazeGameObjects(int totalRows, int totalCols) {
		if (!m_pMazeGrid) return;
		
		m_RemainingPellets = 0; // reset

		for (int r = 0; r < totalRows; ++r) {
			for (int c = 0; c < totalCols; ++c) {
				TileType tileId = m_pMazeGrid->GetTileType(c, r);
				glm::vec3 centerPos = m_pMazeGrid->GridToWorldCenter(c, r);

				switch (tileId)
				{
				case TileType::Wall: // 8x8
				{
					auto wall = std::make_unique<dae::GameObject>();
					wall->GetTransform().SetWorldPosition(centerPos); 
					wall->AddComponent<dae::RenderComponent>("Tiles/Wall.png");

					m_Walls.push_back(wall.get());
					m_pActiveScene->Add(std::move(wall));
				}
				break;

				case TileType::Pellet: // 4x4
				{
					auto dot = std::make_unique<dae::GameObject>();
					dot->GetTransform().SetWorldPosition(centerPos); 
					dot->AddComponent<dae::RenderComponent>("Tiles/Pellet.png");
					dot->AddComponent<dae::BoxColliderComponent>(4.f, 4.f);
					dot->AddComponent<PelletComponent>(10, false)->AddObserver(this);

					m_Pellets.push_back(dot.get()); 
					m_pActiveScene->Add(std::move(dot));

					m_RemainingPellets++;
				}
				break;

				case TileType::PowerPellet: // 8x8
				{
					auto powerPellet = std::make_unique<dae::GameObject>();
					powerPellet->GetTransform().SetWorldPosition(centerPos); 
					powerPellet->AddComponent<dae::RenderComponent>("Tiles/PowerPellet.png");
					powerPellet->AddComponent<dae::BoxColliderComponent>(8.f, 8.f);
					powerPellet->AddComponent<PelletComponent>(50, true)->AddObserver(this);

					m_Pellets.push_back(powerPellet.get());
					m_pActiveScene->Add(std::move(powerPellet));

					m_RemainingPellets++;
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

	void GameManager::NextLevelLogic() {
		m_Level++;

		switch (m_Level) {
		case 0:
			std::cout << "Loading PINK maze..." << std::endl;
			LoadMaze(Maze::PinkMaze);
			break;
		case 1:
			std::cout << "Loading LIGHT BLUE maze..." << std::endl;
			LoadMaze(Maze::LightBlueMaze);
			break;
		case 2:
			std::cout << "Loading ORANGE maze..." << std::endl;
			LoadMaze(Maze::OrangeMaze);
			break;
		case 3:		
		default:
			FinishGame();
			break;
		} 
	}

	void GameManager::FinishGame() {

	}
}