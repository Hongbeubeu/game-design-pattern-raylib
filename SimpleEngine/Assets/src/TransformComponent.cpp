#include "TransformComponent.h"

#include "raymath.h"

TransformComponent::TransformComponent()
{
	transform.translation = { 0.0f, 0.0f, 0.0f };
	transform.rotation = QuaternionIdentity();
	transform.scale = { 1.0f, 1.0f, 1.0f };
}

TransformComponent::~TransformComponent()
= default;

Transform& TransformComponent::GetTransform()
{
	return transform;
}

void TransformComponent::SetTransform(const Transform& newTransform)
{
	transform = newTransform;
}

void TransformComponent::SetPosition(const Vector3& position)
{
	transform.translation = position;
}

void TransformComponent::SetRotation(const Quaternion& rotation)
{
	transform.rotation = rotation;
}

void TransformComponent::SetScale(const Vector3& scale)
{
	transform.scale = scale;
}

void TransformComponent::Update(GameObject* gameObject, float deltaTime)
{
}
