#include "MsPacman.h"

#include "SceneManager.h"
#include "ResourceManager.h"
#include "GameObject.h"
#include "Scene.h"
#include "InputManager.h"

#include "KeyboardBinding.h"

#include "Components/RenderComponent.h"
#include "Components/TextComponent.h"
#include "Components/FPSComponent.h"
#include "Components/RotationComponent.h"
#include "Components/ImGuiComponent.h"
#include "Components/DisplayPointsComponent.h"
#include "Components/PointsComponent.h"

void MsPacman::Initialize() {
	auto& scene = dae::SceneManager::GetInstance().CreateScene();
	auto& input = dae::InputManager::GetInstance();

	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);

	// Background
	auto go = std::make_unique<dae::GameObject>();
	go->AddComponent<dae::RenderComponent>("background.png");
	scene.Add(std::move(go));

	// DAE logo (temp placeholder player)
	go = std::make_unique<dae::GameObject>();
	go->AddComponent<dae::RenderComponent>("logo.png");
	go->SetPosition(358, 180);
	go->AddComponent<dae::PointsComponent>();
	
	// Bind bindings to play
	input.AddBinding(std::make_unique<dae::KeyboardBinding>(SDL_SCANCODE_W, std::make_unique<dae::AddPointsCommand>(go.get(), 1), dae::KeyState::OnPress));
	

	// Display points
	auto DispalyPoints = std::make_unique<dae::GameObject>();
	DispalyPoints->SetPosition(10.f, 325.f);
	DispalyPoints->AddComponent<dae::TextComponent>("", font);
	DispalyPoints->AddComponent<dae::DisplayPointsComponent>(go.get());
	scene.Add(std::move(DispalyPoints));

	scene.Add(std::move(go));

	// Text
	auto to = std::make_unique<dae::GameObject>();
	to->AddComponent<dae::TextComponent>("Programming 4 Assignment", font);
	//to->SetColor({ 255, 255, 0, 255 }); // TODO add function Setcolor
	to->SetPosition(292, 20);
	scene.Add(std::move(to));

	// FPS counter
	to = std::make_unique<dae::GameObject>();
	to->AddComponent<dae::FPSComponent>(font);
	scene.Add(std::move(to));

	//// Rotating objects
	//auto rotating = std::make_unique<dae::GameObject>();
	//rotating->AddComponent<dae::RenderComponent>("logo.png");
	//rotating->SetPosition(200, 300);
	//rotating->AddComponent<dae::RotationComponent>(270.0f, false);

	//auto rotating2 = std::make_unique<dae::GameObject>();
	//rotating2->AddComponent<dae::RenderComponent>("logo.png");
	//rotating2->SetPosition(100, 0);
	//rotating2->SetParent(rotating.get(), false);
	//rotating2->AddComponent<dae::RotationComponent>(360.0f, true);
	//
	//scene.Add(std::move(rotating)); // if added to scene too early, you cant add childeren due to it becoming a nullptr after std::move()
	//scene.Add(std::move(rotating2));



	// ImGui 
	auto gui = std::make_unique<dae::GameObject>();
	gui->AddComponent<dae::ImGuiComponent>();
	scene.Add(std::move(gui));
}