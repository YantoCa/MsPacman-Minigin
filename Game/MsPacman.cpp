#include "MsPacman.h"

#include "SceneManager.h"
#include "ResourceManager.h"
#include "GameObject.h"
#include "Scene.h"
#include "InputManager.h"

#include "KeyboardBinding.h"
#include "ControllerBinding.h"
#include "Commands.h"

#include "Components/RenderComponent.h"
#include "Components/TextComponent.h"
#include "Components/FPSComponent.h" 
#include "Components/MovementComponent.h"
#include "Components/ImGuiComponent.h"
#include "Components/DisplayPointsComponent.h"
#include "Components/PointsComponent.h"

using namespace game;

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
		auto* pPointsCompP1 = p1->AddComponent<PointsComponent>(); // Grab pointer to subject to attach it to observer
		p1->AddComponent<MovementComponent>();

		// Bind bindings to player 1
		input.AddBinding(std::make_unique<dae::KeyboardBinding>(SDL_SCANCODE_SPACE, std::make_unique<AddPointsCommand>(*p1, 1), dae::KeyState::OnPress)); 
		input.AddBinding(std::make_unique<dae::KeyboardBinding>(SDL_SCANCODE_W, std::make_unique<MoveCommand>(*p1, glm::vec3{ 0.f, -1.f, 0.f }), dae::KeyState::OnHold));
		input.AddBinding(std::make_unique<dae::KeyboardBinding>(SDL_SCANCODE_S, std::make_unique<MoveCommand>(*p1, glm::vec3{ 0.f, 1.f, 0.f }), dae::KeyState::OnHold));
		input.AddBinding(std::make_unique<dae::KeyboardBinding>(SDL_SCANCODE_A, std::make_unique<MoveCommand>(*p1, glm::vec3{ -1.f, 0.f, 0.f }), dae::KeyState::OnHold));
		input.AddBinding(std::make_unique<dae::KeyboardBinding>(SDL_SCANCODE_D, std::make_unique<MoveCommand>(*p1, glm::vec3{ 1.f, 0.f, 0.f }), dae::KeyState::OnHold));

		// Observers Player 1
			// Display points UI
			auto DispalyPoints = std::make_unique<dae::GameObject>();
				DispalyPoints->SetPosition(10.f, 325.f);
				DispalyPoints->AddComponent<dae::TextComponent>("Score: 0", font);
				auto* pDisplayObs = DispalyPoints->AddComponent<DisplayPointsComponent>();

		// Couple Subjet and observer
		pPointsCompP1->AddObserver(pDisplayObs);

		// Empty out
		scene.Add(std::move(DispalyPoints));
		scene.Add(std::move(p1));

	// Player 2 (Gamepad dummy)
		auto p2 = std::make_unique<dae::GameObject>();
		p2->AddComponent<dae::RenderComponent>("logo.png");
		p2->SetPosition(358, 180);
		auto* pPointsCompP2 = p2->AddComponent<PointsComponent>(); // Grab pointer to subject to attach it to observer
		p2->AddComponent<MovementComponent>();

		// Bind bindings to player 2
		auto& controller0 = input.AddController(0);
		input.AddBinding(std::make_unique<dae::ControllerBinding>(controller0, dae::Gamepad::ControllerButton::GAMEPAD_A, std::make_unique<AddPointsCommand>(*p2, 1), dae::KeyState::OnPress));
		input.AddBinding(std::make_unique<dae::ControllerBinding>(controller0, dae::Gamepad::ControllerButton::GAMEPAD_DPAD_UP, std::make_unique<MoveCommand>(*p2, glm::vec3{ 0.f, -1.f, 0.f }), dae::KeyState::OnHold));
		input.AddBinding(std::make_unique<dae::ControllerBinding>(controller0, dae::Gamepad::ControllerButton::GAMEPAD_DPAD_DOWN, std::make_unique<MoveCommand>(*p2, glm::vec3{ 0.f, 1.f, 0.f }), dae::KeyState::OnHold));
		input.AddBinding(std::make_unique<dae::ControllerBinding>(controller0, dae::Gamepad::ControllerButton::GAMEPAD_DPAD_LEFT, std::make_unique<MoveCommand>(*p2, glm::vec3{ -1.f, 0.f, 0.f }), dae::KeyState::OnHold));
		input.AddBinding(std::make_unique<dae::ControllerBinding>(controller0, dae::Gamepad::ControllerButton::GAMEPAD_DPAD_RIGHT, std::make_unique<MoveCommand>(*p2, glm::vec3{ 1.f, 0.f, 0.f }), dae::KeyState::OnHold));

		// Observers Player 2
			// Display points UI
				DispalyPoints = std::make_unique<dae::GameObject>();
				DispalyPoints->SetPosition(10.f, 375.f);
				DispalyPoints->AddComponent<dae::TextComponent>("Score: 0", font);
				pDisplayObs = DispalyPoints->AddComponent<DisplayPointsComponent>();

		// Couple Subject and observer
		pPointsCompP2->AddObserver(pDisplayObs);

		// Empty out
		scene.Add(std::move(DispalyPoints));
		scene.Add(std::move(p2));
 
	// FPS counter
		auto fps = std::make_unique<dae::GameObject>();
		fps->AddComponent<FPSComponent>(font);
		scene.Add(std::move(fps));

	// ImGui 
		auto gui = std::make_unique<dae::GameObject>();
		gui->AddComponent<dae::ImGuiComponent>();
		scene.Add(std::move(gui));
}