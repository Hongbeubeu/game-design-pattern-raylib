#pragma once
#ifndef SCENE_H
#define SCENE_H
#include "GameObject.h"

class Scene
{
public:
	Scene();
	void AddGameObject(const std::shared_ptr<GameObject>& gameObject);
	void RemoveGameObject(const std::shared_ptr<GameObject>& gameObject);
	void Update(float deltaTime);
	void FixedUpdate() const;
	void Draw() const;
	void Unload();
private:
	std::vector<std::shared_ptr<GameObject>> gameObjects;
	float fixedUpdateTimer;
	const float fixedUpdateInterval = 0.02f;
};

#endif


