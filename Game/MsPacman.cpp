#include "MsPacman.h"

#include "SceneManager.h"
#include "ResourceManager.h"
#include "GameObject.h"
#include "Scene.h"
#include "InputManager.h"

#include "KeyboardBinding.h"
#include "ControllerBinding.h"

#include "Components/RenderComponent.h"
#include "Components/TextComponent.h"
#include "Components/FPSComponent.h"
#include "Components/RotationComponent.h"
#include "Components/MovementComponent.h"
#include "Components/ImGuiComponent.h"
#include "Components/DisplayPointsComponent.h"
#include "Components/PointsComponent.h"

void MsPacman::Initialize() {
	auto& scene = dae::SceneManager::GetInstance().CreateScene();
	auto& input = dae::InputManager::GetInstance();

	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);

	// Background
		auto bg = std::make_unique<dae::GameObject>();
		bg->AddComponent<dae::RenderComponent>("background.png");
		scene.Add(std::move(bg));

	// Player 1 (Keyboard dummy)
		auto p1 = std::make_unique<dae::GameObject>();
		p1->AddComponent<dae::RenderComponent>("logo.png");
		p1->SetPosition(358, 180);
		auto* pPointsCompP1 = p1->AddComponent<dae::PointsComponent>(); // Grab pointer to subject to attach it to observer
		p1->AddComponent<dae::MovementComponent>();

		// Bind bindings to player 1
		input.AddBinding(std::make_unique<dae::KeyboardBinding>(SDL_SCANCODE_W, std::make_unique<dae::AddPointsCommand>(*p1, 1), dae::KeyState::OnHold)); 

		// Observers Player 1
			// Display points UI
			auto DispalyPoints = std::make_unique<dae::GameObject>();
				DispalyPoints->SetPosition(10.f, 325.f);
				DispalyPoints->AddComponent<dae::TextComponent>("Score: 0", font);
				auto* pDisplayObs = DispalyPoints->AddComponent<dae::DisplayPointsComponent>();

		// Couple Subjet and observer
		pPointsCompP1->AddObserver(pDisplayObs);

		// Empty out
		scene.Add(std::move(DispalyPoints));
		scene.Add(std::move(p1));

	// Player 2 (Gamepad dummy)
		auto p2 = std::make_unique<dae::GameObject>();
		p2->AddComponent<dae::RenderComponent>("logo.png");
		p2->SetPosition(358, 180);
		auto* pPointsCompP2 = p2->AddComponent<dae::PointsComponent>(); // Grab pointer to subject to attach it to observer
		p2->AddComponent<dae::MovementComponent>();

		// Bind bindings to player 2
		auto& controller0 = input.AddController(0);
		input.AddBinding(std::make_unique<dae::ControllerBinding>(controller0, dae::Gamepad::ControllerButton::GAMEPAD_A, std::make_unique<dae::AddPointsCommand>(*p2, 1), dae::KeyState::OnHold));

		// Observers Player 2
			// Display points UI
				DispalyPoints = std::make_unique<dae::GameObject>();
				DispalyPoints->SetPosition(10.f, 375.f);
				DispalyPoints->AddComponent<dae::TextComponent>("Score: 0", font);
				pDisplayObs = DispalyPoints->AddComponent<dae::DisplayPointsComponent>();

		// Couple Subject and observer
		pPointsCompP2->AddObserver(pDisplayObs);

		// Empty out
		scene.Add(std::move(DispalyPoints));
		scene.Add(std::move(p2));
 
	// FPS counter
		auto fps = std::make_unique<dae::GameObject>();
		fps->AddComponent<dae::FPSComponent>(font);
		scene.Add(std::move(fps));

	// ImGui 
		auto gui = std::make_unique<dae::GameObject>();
		gui->AddComponent<dae::ImGuiComponent>();
		scene.Add(std::move(gui));
}