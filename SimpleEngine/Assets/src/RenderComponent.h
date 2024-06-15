#pragma once
#ifndef RENDERCOMPONENT_H
#define RENDERCOMPONENT_H
#include "DrawableComponent.h"
#include "raylib.h"

class RenderComponent : public DrawableComponent
{
public:
	RenderComponent(Model model);
	~RenderComponent() override;

	void SetTexture(Texture2D texture); // Set the texture of the model
	void SetMaterial(Material material); // Set the material of the model
	void Update(GameObject* gameObject, float deltaTime) override;
	void Draw(GameObject* gameObject) override;
private:
	Model model;
	Texture2D texture;
	Material material;
	bool hasTexture;
	bool hasMaterial;
};

#endif


