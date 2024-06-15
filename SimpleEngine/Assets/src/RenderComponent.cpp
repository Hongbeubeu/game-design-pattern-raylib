#include "RenderComponent.h"
#include "GameObject.h"
#include "raymath.h"
#include "TransformComponent.h"

RenderComponent::RenderComponent(Model model) : model(model), hasTexture(false), hasMaterial(false) {}

RenderComponent::~RenderComponent() {
	UnloadModel(model);
	if (hasTexture)
	{
		UnloadTexture(texture);
	}
	if (hasMaterial)
	{
		UnloadMaterial(material);
	}
}

void RenderComponent::SetTexture(Texture2D texture)
{
	this->texture = texture;
	hasTexture = true;
	model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;
}

void RenderComponent::SetMaterial(Material material)
{
	this->material = material;
	hasMaterial = true;
	model.materials[0] = material;
}

void RenderComponent::Update(GameObject* gameObject, float deltaTime)
{
}

void RenderComponent::Draw(GameObject* gameObject) {
	TransformComponent* transformComponent = gameObject->GetComponent<TransformComponent>();
	if (transformComponent)
	{
		Transform transform = transformComponent->GetTransform();
		Matrix matTransform = MatrixIdentity();
		matTransform = MatrixMultiply(matTransform, MatrixTranslate(transform.translation.x, transform.translation.y, transform.translation.z));
		matTransform = MatrixMultiply(matTransform, MatrixRotateXYZ(QuaternionToEuler(transform.rotation)));
		matTransform = MatrixMultiply(matTransform, MatrixScale(transform.scale.x, transform.scale.y, transform.scale.z));

		DrawModelEx(model, transform.translation, Vector3{1,0,0},QuaternionToEuler(transform.rotation).x, transform.scale, WHITE);
	}
}
