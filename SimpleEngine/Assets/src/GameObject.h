#pragma once
#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include <memory>
#include <vector>

#include "Component.h"
#include "DrawableComponent.h"

class TransformComponent;

class GameObject
{
public:
	GameObject();
	virtual ~GameObject();

	// Lifecycle methods
	virtual void Awake();
	virtual void Start();
	virtual void OnEnable();
	virtual void OnDisable();
	virtual void FixedUpdate();
	virtual void Update(float deltaTime);
	virtual void LateUpdate();
	virtual void OnDestroy();

	void AddComponent(std::unique_ptr<Component> component);

	template<typename T>
	T* GetComponent() const
	{
		for (const auto& component : components)
		{
			if (T* target = dynamic_cast<T*>(component.get()))
			{
				return target;
			}
		}

		return nullptr;
	}

	void AddChild(const std::shared_ptr<GameObject>& child);
	void RemoveChild(const std::shared_ptr<GameObject>& child);
	const std::vector<std::shared_ptr<GameObject>>& GetChildren() const;

	void Draw();
	bool IsActive() const;
	void SetActive(bool active);

private:
	std::unique_ptr<TransformComponent> transform;
	std::vector<std::unique_ptr<Component>> components;
	std::vector<std::unique_ptr<DrawableComponent>> drawableComponents;
	std::vector<std::shared_ptr<GameObject>> children;
	bool isActive;
};

#endif
