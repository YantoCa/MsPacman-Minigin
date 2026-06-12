#pragma once
// Central place for all the enums / and possibly later structs

namespace game {
	// GridComponent
	enum class TileType : int {
		Empty = 0,
		Wall = 1,
		Pellet = 2,
		PowerPellet = 3,
		Player1Spawn = 5,
		Player2Spawn = 6,
		GhostSpawn = 7
	};

	// GameManager
	enum class GameState {
		StartScreen,
		Paused,
		Game,
		EndScreen
	};
	enum class Maze {
		PinkMaze,
		LightBlueMaze,
		OrangeMaze
	};
}