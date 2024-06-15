#pragma once
#ifndef COMPONENT_H
#define COMPONENT_H

class GameObject;

class Component
{
public:
	virtual ~Component() = default;
	virtual void Update(GameObject* gameObject, float deltaTime) = 0;
};
#endif
