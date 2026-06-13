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
#include "FSMComponent.h"
#include "GhostFrightenedState.h"
#include "GhostComponent.h"
#include "GhostChaseState.h"
#include <iostream>

namespace game {
	GameManager::GameManager(dae::GameObject& owner, dae::Scene* scene)
		: dae::Component(owner), m_pActiveScene{ scene } {
		LoadMaze(game::Maze::PinkMaze); // load the starting maze

		m_Lives = m_MaxLives;
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
					return;
				}
			}

			// Collision Ghosts
			for (auto* ghost : m_Ghosts)
			{
				if (!ghost || ghost->IsMarkedForDeletion()) continue;

				auto* ghostCollider = ghost->GetComponent<dae::BoxColliderComponent>();
				if (!ghostCollider) continue;

				if (playerCollider->IsOverlapping(*ghostCollider))
				{ 
					bool isFrightened = false;
					if (auto* fsm = ghost->GetComponent<dae::FSMComponent>())
					{ 
						if (dynamic_cast<GhostFrightenedState*>(fsm->GetCurrentState()))
						{
							isFrightened = true;
						}
					}

					if (isFrightened)
					{   
						m_MasterScore += 200;  
						Notify(*GetOwner(), "ScoreChanged"); 
						 
						if (auto* fsm = ghost->GetComponent<dae::FSMComponent>())
						{
							fsm->ChangeState(std::make_unique<GhostChaseState>(*ghost, this));
						}
						 
						auto& ghostpawns = m_pMazeGrid->GetGhostSpawnPoints();
						if (!ghostpawns.empty())
						{ 
							size_t ghostIdx = 0;
							for (size_t i = 0; i < m_Ghosts.size(); ++i)
							{
								if (m_Ghosts[i] == ghost) { ghostIdx = i; break; }
							}

							glm::vec3 spawnPos = (ghostIdx < ghostpawns.size()) ? ghostpawns[ghostIdx] : ghostpawns[0];
							ghost->GetTransform().SetWorldPosition(spawnPos);

							if (auto* pGridMove = ghost->GetComponent<GridMovementComponent>())
							{
								pGridMove->SnapToGrid();
							}
						}
					}
					else
					{ 
						PlayerDied();
						return;
					}
				}
			}
		}
	}

	void GameManager::LoadMaze(const Maze& newMaze) {
		ClearUpMaze();

		std::string csvPath;
		std::string texturePath;

		switch (newMaze)
		{
		case game::Maze::PinkMaze: 
			csvPath = "Data/Levels/Level1.csv";
			texturePath = "Levels/Level1_Background.png";
			break;
		case game::Maze::LightBlueMaze: 
			csvPath = "Data/Levels/Level2.csv";
			texturePath = "Levels/Level2_Background.png";
			break;
		case game::Maze::OrangeMaze:
			csvPath = "Data/Levels/Level3.csv";
			texturePath = "Levels/Level3_Background.png";
			break;
		default:
			break;
		}

		if (csvPath.empty() && texturePath.empty()) return; // make sure something needs to load

		auto matrix = LevelLoader::ParseCSV(csvPath);
		if (matrix.empty()) return;

		int totalRows = static_cast<int>(matrix.size());
		int totalCols = static_cast<int>(matrix[0].size());

		// Create a gird gameobject
		auto gridObject = std::make_unique<dae::GameObject>();
		gridObject->GetTransform().SetWorldPosition(GetOwner()->GetTransform().GetWorldPosition());
		gridObject->AddComponent<dae::RenderComponent>(texturePath);
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
			 
			m_MasterScore += pPellet->GetPointValue();
			m_RemainingPellets--;

			// UI Notifys
			Notify(*GetOwner(), "ScoreChanged");
			 
			if (m_RemainingPellets <= 0)
			{
				NextLevelLogic();
				return;
			}

			if (pPellet->IsPowerPellet())
			{
				TriggerGhostsFrightenedMode();
			}
		}
	}

	void GameManager::ResetMaze() {
		if (!m_pMazeGrid) return;

		m_RemainingPellets = 0;
		for (auto* pellet : m_Pellets) {
			if (!pellet) continue;

			if (auto* pPelletComp = pellet->GetComponent<game::PelletComponent>()) {
				pPelletComp->Reset();
			}

			m_RemainingPellets++; 
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
		if (ghostpawns.empty()) return;  

		for (size_t i = 0; i < m_Ghosts.size(); ++i)
		{
			if (!m_Ghosts[i]) continue;
			 
			glm::vec3 spawnPos = (i < ghostpawns.size()) ? ghostpawns[i] : ghostpawns[0];
			m_Ghosts[i]->GetTransform().SetWorldPosition(spawnPos);

			if (auto* pGridMove = m_Ghosts[i]->GetComponent<GridMovementComponent>())
			{
				pGridMove->SetGrid(m_pMazeGrid);  
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
	void GameManager::ClearUpMaze(bool isGameOver) {
		for (auto* player : m_Players) {
			if (player) {
				if (auto* pGridMove = player->GetComponent<GridMovementComponent>()) {
					pGridMove->SetGrid(nullptr);  
					if (isGameOver) pGridMove->SetActive(false);
				}
				if (isGameOver) {
					if (auto* pRender = player->GetComponent<dae::RenderComponent>()) {
						pRender->SetActive(false);
					}
				}
			}
		}
		if (isGameOver) m_Players.clear();
		 
		for (auto* pellet : m_Pellets) { if (pellet) pellet->Destroy(); }
		for (auto* wall : m_Walls) { if (wall)    wall->Destroy(); }
		for (auto* fruit : m_Fruit) { if (fruit)   fruit->Destroy(); }

		m_Pellets.clear();
		m_Walls.clear();
		m_Fruit.clear();
		 
		for (auto* ghost : m_Ghosts) {
			if (ghost) {
				if (auto* pGridMove = ghost->GetComponent<GridMovementComponent>()) {
					pGridMove->SetGrid(nullptr);
					if (isGameOver) pGridMove->SetActive(false);
				}
				if (isGameOver) {
					if (auto* pFsm = ghost->GetComponent<dae::FSMComponent>()) {
						pFsm->SetActive(false);
					}
					ghost->Destroy();  
				}
			}
		}
		if (isGameOver) m_Ghosts.clear();
		 
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
				default:
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

	void GameManager::TriggerGhostsFrightenedMode() {
		for (auto* ghost : m_Ghosts)
		{
			if (!ghost || ghost->IsMarkedForDeletion()) continue;

			if (auto* fsm = ghost->GetComponent<dae::FSMComponent>())
			{ 
				fsm->ChangeState(std::make_unique<GhostFrightenedState>(*ghost,this));
			}
		}
	}

	void GameManager::FinishGame() {
		ClearUpMaze(true);
	}

	void GameManager::PlayerDied() {
		m_Lives--;

		Notify(*GetOwner(), "LivesChanged"); // UI update

		if (m_Lives <= 0) {
			std::cout << "Game Over!" << std::endl;
			FinishGame();
		}
		else {
			std::cout << "Life lost! Remaining: " << m_Lives << std::endl;
			 
			ResetPlayers();
			ResetGhosts();
		}
	}
}