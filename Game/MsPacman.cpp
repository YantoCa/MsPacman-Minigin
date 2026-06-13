#include "MsPacman.h"

#include "SceneManager.h"
#include "ResourceManager.h"
#include "GameObject.h"
#include "Scene.h"
#include "InputManager.h"
#include "ServiceLocator.h"

#include "KeyboardBinding.h"
#include "ControllerBinding.h"
#include "Commands.h"

#include "SDLSoundSystem.h"
#include "LoggingSoundSystem.h"

#include "Components/RenderComponent.h"
#include "Components/TextComponent.h"
#include "Components/FPSComponent.h" 
#include "Components/ImGuiComponent.h"
#include "Components/DisplayPointsComponent.h"
#include "Components/PointsComponent.h"
#include "Components/BoxColliderComponent.h"

#include "LevelLoader.h"
#include "GridMovementComponent.h"
#include "GameManager.h"

using namespace game;

void MsPacman::Initialize() {
	SetupSound();

	auto& scene = dae::SceneManager::GetInstance().CreateScene();
	auto& input = dae::InputManager::GetInstance();

	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);

	//// Level loader
	auto GM = std::make_unique<dae::GameObject>();
	auto* manager = GM->AddComponent<GameManager>(&scene);

	// Player 1 (Keyboard dummy)
		auto p1 = std::make_unique<dae::GameObject>();
		p1->AddComponent<dae::RenderComponent>("Characters/MsPacman.png");
		p1->AddComponent<dae::BoxColliderComponent>(8.f,8.f); 
		p1->AddComponent<GridMovementComponent>(manager->GetMazeGrid()); 


		// Bind bindings to player 1
		input.AddBinding(std::make_unique<dae::KeyboardBinding>(SDL_SCANCODE_SPACE, std::make_unique<SkipLevelCommand>(*manager), dae::KeyState::OnPress));
		input.AddBinding(std::make_unique<dae::KeyboardBinding>(SDL_SCANCODE_W, std::make_unique<MoveCommand>(*p1, glm::ivec2{ 0.f, -1.f}), dae::KeyState::OnHold));
		input.AddBinding(std::make_unique<dae::KeyboardBinding>(SDL_SCANCODE_S, std::make_unique<MoveCommand>(*p1, glm::ivec2{ 0.f, 1.f}), dae::KeyState::OnHold));
		input.AddBinding(std::make_unique<dae::KeyboardBinding>(SDL_SCANCODE_A, std::make_unique<MoveCommand>(*p1, glm::ivec2{ -1.f, 0.f}), dae::KeyState::OnHold));
		input.AddBinding(std::make_unique<dae::KeyboardBinding>(SDL_SCANCODE_D, std::make_unique<MoveCommand>(*p1, glm::ivec2{ 1.f, 0.f}), dae::KeyState::OnHold));
		
		// Bind player to gamemanager and reset position
		manager->AddPlayer(p1.get());
		scene.Add(std::move(p1));

	// UI Score;
		auto UIScore = std::make_unique<dae::GameObject>();
		//
		UIScore->AddComponent<dae::TextComponent>("score : ", font);
		auto* pDisplacScoreObs = UIScore->AddComponent<DisplayPointsComponent>();
		
		// observe the player points
		manager->AddObserver(pDisplacScoreObs);
		scene.Add(std::move(UIScore));


	// Sceneloader gone into scene
	scene.Add(std::move(GM)); 
}

void MsPacman::SetupSound() {
	//// Sound
	auto realAudioSys = std::make_unique<dae::SDLSoundSystem>(); // using sdl

	realAudioSys->LoadSound(0, "Data/Dummy_Sound.mp3");

	auto decoratedSys = std::make_unique<dae::LoggingSoundSystem>(std::move(realAudioSys)); // wrapper 

	dae::ServiceLocator::RegisterSoundSystem(std::move(decoratedSys));
}