#include <algorithm>
#include "Scene.h"

using namespace dae;

void Scene::Add(std::unique_ptr<GameObject> object)
{
	assert(object != nullptr && "Cannot add a null GameObject to the scene.");
	m_objects.emplace_back(std::move(object));
}

void Scene::Remove(GameObject& object)
{ 
	object.MarkForDeletion();
}

void Scene::RemoveAll()
{
	m_objects.clear(); 
}

void Scene::CleanUpMarkedForDeletion() {
	m_objects.erase(
		std::remove_if(m_objects.begin(), m_objects.end(),
			[](const std::unique_ptr<GameObject>& ptr) {
				return ptr->IsMarkedForDeletion();
			}),
		m_objects.end()
	);
}

void Scene::Update(float deltaTime)
{
	for(auto& object : m_objects)
	{
		object->Update(deltaTime);
	}

	CleanUpMarkedForDeletion();
}

void Scene::LateUpdate(float deltaTime)
{
	for (auto& object : m_objects)
	{
		object->LateUpdate(deltaTime);
	}

	CleanUpMarkedForDeletion();
}

void Scene::Render() const
{
	for (const auto& object : m_objects)
	{
		object->Render();
	}
}

