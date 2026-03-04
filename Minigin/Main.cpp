#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#if _DEBUG && __has_include(<vld.h>)
#include <vld.h>
#endif

#include "Minigin.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "RenderComponent.h"
#include "GameObject.h"
#include "TextComponent.h"
#include "FPSComponent.h"
#include "RotationComponent.h"
#include "ImGuiComponent.h"

#include <filesystem>
namespace fs = std::filesystem;

static void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene();
	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);

	// Background
	auto go = std::make_unique<dae::GameObject>();
	go->AddComponent<dae::RenderComponent>("background.png");
	scene.Add(std::move(go));

	// DAE logo
	go = std::make_unique<dae::GameObject>();
	go->AddComponent<dae::RenderComponent>("logo.png");
	go->SetPosition(358, 180);
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

	// Rotating objects
	auto rotating = std::make_unique<dae::GameObject>();
	rotating->AddComponent<dae::RenderComponent>("logo.png");
	rotating->SetPosition(200, 300);
	rotating->AddComponent<dae::RotationComponent>(270.0f, false);

	auto rotating2 = std::make_unique<dae::GameObject>();
	rotating2->AddComponent<dae::RenderComponent>("logo.png");
	rotating2->SetPosition(100, 0);
	rotating2->SetParent(rotating.get(), false);
	rotating2->AddComponent<dae::RotationComponent>(360.0f, true);
	
	scene.Add(std::move(rotating)); // if added to scene too early, you cant add childeren due to it becoming a nullptr after std::move()
	scene.Add(std::move(rotating2));

	// ImGui 
	auto gui = std::make_unique<dae::GameObject>();
	gui->AddComponent<dae::ImGuiComponent>();
	scene.Add(std::move(gui));
}

int main(int, char*[]) {
#if __EMSCRIPTEN__
	fs::path data_location = "";
#else
	fs::path data_location = "./Data/";
	if(!fs::exists(data_location))
		data_location = "../Data/";
#endif
	dae::Minigin engine(data_location);
	engine.Run(load);
    return 0;
}
