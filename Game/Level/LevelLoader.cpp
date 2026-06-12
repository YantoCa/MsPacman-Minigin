#include "LevelLoader.h"
#include "Scene.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "RenderComponent.h" // Utilizing your engine component headers

#include <fstream>
#include <sstream>
#include <iostream>

namespace game {
	bool LevelLoader::LoadLevel(const std::string& csvFilePath, dae::Scene& targetScene)
	{
		std::ifstream file(csvFilePath);
		if (!file.is_open())
		{
			std::cerr << "LevelLoader Error: Failed to open layout file: " << csvFilePath << "\n";
			return false;
		}

		std::string line;
		int rowCounter = 0;

		// Read line by line
		while (std::getline(file, line))
		{
			std::stringstream lineStream(line);
			std::string cellValue;
			int columnCounter = 0;

			// Split csv by ;
			while (std::getline(lineStream, cellValue, ';'))
			{
				if (!cellValue.empty())
				{
					try
					{
						int tileId = std::stoi(cellValue);
						SpawnTile(tileId, columnCounter, rowCounter, targetScene);
					}
					catch (const std::exception&)
					{
						continue;
					}
				}
				columnCounter++;
			}
			rowCounter++;
		}

		return true;
	}

	void LevelLoader::SpawnTile(int tileId, int column, int row, dae::Scene& scene)
	{
		// Calculate coordinates based on grid row and column positions
		float xPos = column * TILE_SIZE;
		float yPos = row * TILE_SIZE;

		switch (tileId)
		{
		case 1: // Wall 24x24
		{
			auto wall = std::make_unique<dae::GameObject>();
			wall->GetTransform().SetWorldPosition(glm::vec3{ xPos, yPos, 0.0f });
			wall->AddComponent<dae::RenderComponent>("Wall.png");
			scene.Add(std::move(wall)); 
		}
		break;

		case 2: // Pac-Dot, 8x8
		{ 
			auto dot = std::make_unique<dae::GameObject>(); 
			dot->GetTransform().SetWorldPosition(glm::vec3{ xPos + PADDING_8x8, yPos + PADDING_8x8, 0.0f });
			dot->AddComponent<dae::RenderComponent>("Tiles/Pellet.png"); 
			scene.Add(std::move(dot)); 
		}
		break;

		case 3: // Power Pellet 8x8
		{
			auto powerPellet = std::make_unique<dae::GameObject>();
			powerPellet->GetTransform().SetWorldPosition(glm::vec3{ xPos + PADDING_8x8, yPos + PADDING_8x8, 0.0f });
			powerPellet->AddComponent<dae::RenderComponent>("Tiles/PowerPellet.png");
			scene.Add(std::move(powerPellet));

			std::cout << "LevelLoader: creating power pellet\n";
		}
		break;

		case 5: // Ms. Pac-man 16x16
		{ // later create a central spawnpoint. where players can respawn on death or on loadup. (also for multiplayer)
			auto player = std::make_unique<dae::GameObject>();
			player->GetTransform().SetWorldPosition(glm::vec3{ xPos + PADDING_16x16 , yPos + PADDING_16x16, 0.0f });
			player->AddComponent<dae::RenderComponent>("Characters/MsPacman.png"); 

			scene.Add(std::move(player));

			std::cout << "LevelLoader: creating MsPacman \n";
		}
		break;

		default: // Anything else
			break;
		}
	}
}