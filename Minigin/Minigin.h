#pragma once
#include <string>
#include <functional>
#include <filesystem>

#include "Game.h" 

namespace dae
{ 
	class Minigin final
	{
		bool m_quit{};
		std::chrono::high_resolution_clock::time_point m_LastTime{};
		std::unique_ptr<Game> m_pGame{};

	public:
		explicit Minigin(const std::filesystem::path& dataPath, std::unique_ptr<Game> game);
		~Minigin();
		void Run();
		void RunOneFrame();

		Minigin(const Minigin& other) = delete;
		Minigin(Minigin&& other) = delete;
		Minigin& operator=(const Minigin& other) = delete;
		Minigin& operator=(Minigin&& other) = delete;
	};
}