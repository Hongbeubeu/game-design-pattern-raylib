#pragma once
#ifndef TRANSFORMCOMPONENT_H
#define TRANSFORMCOMPONENT_H
#include "Component.h"
#include "raylib.h"

class TransformComponent : public Component
{
public:
	TransformComponent();
	~TransformComponent() override;

	Transform& GetTransform();
	void SetTransform(const Transform& newTransform);
	void SetPosition(const Vector3& position);
	void SetRotation(const Quaternion& rotation);
	void SetScale(const Vector3& scale);

	void Update(GameObject* gameObject, float deltaTime) override;

private:
	Transform transform;
};
#endif


