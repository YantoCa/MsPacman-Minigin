#pragma once
#include "Observer.h"

#include "GridComponent.h"

namespace game {
	class GameManager : public dae::Observer {
		enum class GameState {
			StartScreen,
			Paused,
			Game,
			EndScreen
		};
		enum class Level {
			PinkMaze,
			LightBlueMaze,
			OrangeMaze
		};

	public:
		GridComponent* GetLevelGrid() const;
		void LevelTransition(const Level& newLevel);

		void OnNotify(const dae::GameObject& object, Event event) override;

		// Player respawn logic
	private:
		GridComponent* m_pLevelGrid{nullptr};
		std::vector<GameObject*> m_Players{};
		
		// a Player class should handle this
		int m_Score{};
		int m_Lives{};
		//

		// UI
		int m_Highscore{};

	};
}