#include "LevelLoader.h"
#include "Scene.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "RenderComponent.h" 
#include "GridComponent.h"
#include "GridMovementComponent.h"

#include <fstream>
#include <sstream>
#include <iostream>

namespace game {
	bool LevelLoader::LoadLevel(const std::string& csvFilePath, dae::Scene& targetScene)
	{
		auto levelMatrix = ParseCSV(csvFilePath);

		if (levelMatrix.empty()) return false; // empty grid

		auto* pGrid = InitializeGrid(levelMatrix, targetScene);
		if (!pGrid) return false;

		PopulateScene(levelMatrix, targetScene, pGrid);
		
		return true;
	}

	std::vector<std::vector<int>> LevelLoader::ParseCSV(const std::string& csvFilePath)
	{
		std::ifstream file(csvFilePath);
		std::vector<std::vector<int>> matrix;

		if (!file.is_open())
		{
			std::cerr << "LevelLoader Error: Failed to open layout file: " << csvFilePath << "\n";
			return matrix;
		}

		std::string line;
		while (std::getline(file, line))
		{
			std::stringstream lineStream(line);
			std::string cellValue;
			std::vector<int> rowValues;

			while (std::getline(lineStream, cellValue, ';'))
			{
				if (!cellValue.empty())
				{
					try
					{
						rowValues.push_back(std::stoi(cellValue));
					}
					catch (const std::exception&)
					{
						rowValues.push_back(0); // nothing = empty space
					}
				}
			}
			if (!rowValues.empty()) matrix.push_back(rowValues);
		}

		return matrix;
	}

	GridComponent* LevelLoader::InitializeGrid(const std::vector<std::vector<int>>& matrix, dae::Scene& scene)
	{
		int totalRows = static_cast<int>(matrix.size());
		int totalCols = totalRows > 0 ? static_cast<int>(matrix[0].size()) : 0;

		if (totalRows == 0 || totalCols == 0)
		{
			std::cerr << "LevelLoader Error: Matrix boundaries cannot be zero.\n";
			return nullptr;
		}

		auto gridObject = std::make_unique<dae::GameObject>();
		 
		auto* gridComp = gridObject->AddComponent<GridComponent>(totalCols, totalRows, TILE_SIZE);
		 
		for (int r = 0; r < totalRows; ++r)
		{
			for (int c = 0; c < totalCols; ++c)
			{
				gridComp->SetTile(c, r, matrix[r][c]);
			}
		}

		auto* rawGridPtr = gridComp;
		scene.Add(std::move(gridObject));  

		return rawGridPtr;
	}

	void LevelLoader::PopulateScene(const std::vector<std::vector<int>>& matrix, dae::Scene& scene, GridComponent* pGrid)
	{
		int totalRows = pGrid->GetRows();
		int totalCols = pGrid->GetColumns();

		for (int r = 0; r < totalRows; ++r)
		{
			for (int c = 0; c < totalCols; ++c)
			{
				int tileId = matrix[r][c];
				glm::vec3 centerPos = pGrid->GridToWorldCenter(c, r);

				SpawnTile(tileId, centerPos, scene, pGrid);
			}
		}
	}

	void LevelLoader::SpawnTile(int tileId, const glm::vec3& centerPos, dae::Scene& scene, game::GridComponent* pGrid)
	{
		switch (tileId)
		{
		case 1: // Wall 24x24
		{
			auto wall = std::make_unique<dae::GameObject>();
			wall->GetTransform().SetWorldPosition(centerPos);
			wall->AddComponent<dae::RenderComponent>("Tiles/Wall.png");
			scene.Add(std::move(wall)); 
		}
		break;

		case 2: // Pac-Dot, 8x8
		{ 
			auto dot = std::make_unique<dae::GameObject>(); 
			dot->GetTransform().SetWorldPosition(centerPos);
			dot->AddComponent<dae::RenderComponent>("Tiles/Pellet.png"); 
			scene.Add(std::move(dot)); 
		}
		break;

		case 3: // Power Pellet 8x8
		{
			auto powerPellet = std::make_unique<dae::GameObject>();
			powerPellet->GetTransform().SetWorldPosition(centerPos);
			powerPellet->AddComponent<dae::RenderComponent>("Tiles/PowerPellet.png");
			scene.Add(std::move(powerPellet));

			std::cout << "LevelLoader: creating power pellet\n";
		}
		break;

		case 5: // Ms. Pac-man 16x16
		{ // later create a central spawnpoint. where players can respawn on death or on loadup. (also for multiplayer)
			auto player = std::make_unique<dae::GameObject>();
			player->GetTransform().SetWorldPosition(centerPos);
			player->AddComponent<dae::RenderComponent>("Characters/MsPacman.png"); 
			player->AddComponent<GridMovementComponent>(pGrid);

			scene.Add(std::move(player));

			std::cout << "LevelLoader: creating MsPacman \n";
		}
		break;

		default: // Anything else
			break;
		}
	}
}