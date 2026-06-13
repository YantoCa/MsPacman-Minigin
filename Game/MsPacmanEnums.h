#pragma once
// Central place for all the enums / and possibly later structs

namespace game {
	// GridComponent
	enum class TileType : int {
		Empty = 0,
		Wall = 1,
		Pellet = 2,
		PowerPellet = 3,
		Player1Spawn = 4,
		Player2Spawn = 5,
		GhostSpawn = 6,
		WarpTunnel = 7
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

	// GhostChaseState
	enum class GhostType {
		Blinky, 
		Pinky, 
		Inky, 
		Sue
	};
}