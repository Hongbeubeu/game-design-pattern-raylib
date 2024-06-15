#pragma once
#ifndef DRAWABLECOMPONENT_H
#define DRAWABLECOMPONENT_H

#include "Component.h"

class DrawableComponent : public Component
{
public:
	virtual ~DrawableComponent() = default;
	virtual void Draw(GameObject* gameObject) = 0;
};
#endif
