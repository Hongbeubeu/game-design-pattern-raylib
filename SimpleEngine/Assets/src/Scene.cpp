#include "Scene.h"
Scene::Scene() : fixedUpdateTimer(0.0f)
{
}

void Scene::AddGameObject(const std::shared_ptr<GameObject>& gameObject)
{
	gameObjects.push_back(gameObject);
	gameObject->Awake();
}

void Scene::RemoveGameObject(const std::shared_ptr<GameObject>& gameObject)
{
	gameObjects.erase(std::remove(gameObjects.begin(), gameObjects.end(), gameObject), gameObjects.end());
}

void Scene::Update(const float deltaTime)
{
	fixedUpdateTimer += deltaTime;
	while (fixedUpdateTimer >= fixedUpdateInterval)
	{
		 FixedUpdate();
		fixedUpdateTimer -= fixedUpdateInterval;
	}

	for (const auto& gameObject : gameObjects)
	{
		gameObject->Update(deltaTime);
	}
}

void Scene::FixedUpdate() const
{
	for (const auto& gameObject : gameObjects)
	{
		gameObject->FixedUpdate();
	}
}

void Scene::Draw() const
{
	for (const auto& gameObject : gameObjects)
	{
		gameObject->Draw();
	}
}

void Scene::Unload()
{
	for (const auto& gameObject : gameObjects)
	{
		gameObject->OnDestroy();
	}
	gameObjects.clear();
}


