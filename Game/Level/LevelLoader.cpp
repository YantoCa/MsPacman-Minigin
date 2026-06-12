#include "LevelLoader.h"
#include "Scene.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "RenderComponent.h" 
#include "GridComponent.h"

#include <fstream>
#include <sstream>
#include <iostream>

namespace game { 
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
}