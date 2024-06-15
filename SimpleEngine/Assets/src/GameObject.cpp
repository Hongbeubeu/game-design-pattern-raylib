#include "GameObject.h"

#include "TransformComponent.h"

GameObject::GameObject() : isActive(true)
{
	transform = std::make_unique<TransformComponent>();
	AddComponent(std::make_unique<TransformComponent>());
}

GameObject::~GameObject() = default;

void GameObject::Awake()
{
}

void GameObject::Start()
{
}

void GameObject::OnEnable()
{
}

void GameObject::OnDisable()
{
}

void GameObject::FixedUpdate()
{
}

void GameObject::LateUpdate()
{
}

void GameObject::OnDestroy()
{
}

void GameObject::AddComponent(std::unique_ptr<Component> component)
{
	if (DrawableComponent* drawable = dynamic_cast<DrawableComponent*>(component.get()))
	{
		drawableComponents.push_back(std::unique_ptr<DrawableComponent>(drawable));
	}
	components.push_back(std::move(component));
}

void GameObject::AddChild(const std::shared_ptr<GameObject>& child)
{
	children.push_back(child);
}

void GameObject::RemoveChild(const std::shared_ptr<GameObject>& child)
{
	children.erase(std::remove(children.begin(), children.end(), child), children.end());
}

const std::vector<std::shared_ptr<GameObject>>& GameObject::GetChildren() const
{
	return children;
}

void GameObject::Update(const float deltaTime)
{
	if (isActive)
	{
		for (const auto& component : components)
		{
			component->Update(this, deltaTime);
		}

		for (const auto& child : children)
		{
			child->Update(deltaTime);
		}
		LateUpdate();
	}
}

void GameObject::Draw()
{
	if (isActive)
	{
		for (const auto& component : drawableComponents)
		{
			component->Draw(this);
		}

		for (const auto& child : children)
		{
			child->Draw();
		}
	}
}

bool GameObject::IsActive() const
{
	return isActive;
}

void GameObject::SetActive(const bool active)
{
	if (isActive != active)
	{
		isActive = active;
		if (isActive)
		{
			OnEnable();
		}
		else
		{
			OnDisable();
		}
	}
}

