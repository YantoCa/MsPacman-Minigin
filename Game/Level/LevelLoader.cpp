#include "LevelLoader.h"
#include "Scene.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "RenderComponent.h" 
#include "GridComponent.h"
#include "SpawnPointComponent.h"

#include <fstream>
#include <sstream>
#include <iostream>

namespace game {
	GridComponent* LevelLoader::LoadLevel(const std::string& csvFilePath, dae::Scene& targetScene)
	{
		auto levelMatrix = ParseCSV(csvFilePath);

		if (levelMatrix.empty()) return nullptr; // empty grid

		auto* pGrid = InitializeGrid(levelMatrix, targetScene);
		if (!pGrid) return nullptr;

		PopulateScene(levelMatrix, targetScene, pGrid);
		
		return pGrid;
	}

	std::vector<std::vector<TileType>> LevelLoader::ParseCSV(const std::string& csvFilePath)
	{
		std::ifstream file(csvFilePath);
		std::vector<std::vector<TileType>> matrix;

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
			std::vector<TileType> rowValues;

			while (std::getline(lineStream, cellValue, ';'))
			{
				if (!cellValue.empty())
				{
					try
					{
						int rawData = std::stoi(cellValue);
						rowValues.push_back(static_cast<TileType>(rawData));
					}
					catch (const std::exception&)
					{
						rowValues.push_back(TileType::Empty); // nothing = empty space
					}
				}
			}
			if (!rowValues.empty()) matrix.push_back(rowValues);
		}

		return matrix;
	}

	GridComponent* LevelLoader::InitializeGrid(const std::vector<std::vector<TileType>>& matrix, dae::Scene& scene)
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

	void LevelLoader::PopulateScene(const std::vector<std::vector<TileType>>& matrix, dae::Scene& scene, GridComponent* pGrid)
	{
		int totalRows = pGrid->GetRows();
		int totalCols = pGrid->GetColumns();

		for (int r = 0; r < totalRows; ++r)
		{
			for (int c = 0; c < totalCols; ++c)
			{
				TileType tileId = matrix[r][c];
				glm::vec3 centerPos = pGrid->GridToWorldCenter(c, r);

				SpawnTile(tileId, centerPos, scene, pGrid); 
			}
		}
	}

	void LevelLoader::SpawnTile(TileType tileId, const glm::vec3& centerPos, dae::Scene& scene, GridComponent* pGrid)
	{
		if (!pGrid) return;

		switch (tileId)	
		{
		case TileType::Wall: // Wall 24x24
		{
			auto wall = std::make_unique<dae::GameObject>();
			wall->GetTransform().SetWorldPosition(centerPos);
			wall->AddComponent<dae::RenderComponent>("Tiles/Wall.png");
			scene.Add(std::move(wall)); 
		}
		break;

		case TileType::Pellet: // Pac-Dot, 8x8
		{ 
			auto dot = std::make_unique<dae::GameObject>(); 
			dot->GetTransform().SetWorldPosition(centerPos);
			dot->AddComponent<dae::RenderComponent>("Tiles/Pellet.png"); 
			scene.Add(std::move(dot)); 
		}
		break;

		case TileType::PowerPellet: // Power Pellet 8x8
		{
			auto powerPellet = std::make_unique<dae::GameObject>();
			powerPellet->GetTransform().SetWorldPosition(centerPos);
			powerPellet->AddComponent<dae::RenderComponent>("Tiles/PowerPellet.png");
			scene.Add(std::move(powerPellet));
		}
		break;

		case TileType::Player1Spawn: // Ms. Pac-man 16x16 (Player spawnpoint)
		case TileType::Player2Spawn: // Ms. Pac-man 16x16 (Player spawnpoint)
		{  
			pGrid->AddPlayerSpawnPoint(centerPos);
			 
			std::cout << "LevelLoader: created PLAYER SpawnPoint \n";
		}
		break;

		case TileType::GhostSpawn: 
		{
			pGrid->AddGhostSpawnPoint(centerPos);

			std::cout << "LevelLoader: created GHOST SpawnPoint \n";
		}
		break;

		default: // Anything else
			break;
		}
	}
}