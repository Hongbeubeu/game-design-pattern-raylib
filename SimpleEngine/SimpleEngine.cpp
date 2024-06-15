// SimpleEngine.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>

#include "GameObject.h"
#include "raylib.h"
#include "raymath.h"
#include "RenderComponent.h"
#include "Scene.h"
#include "TransformComponent.h"

class MyGameObject : public GameObject
{
public:
	void Awake() override
	{
		SetActive(true);
		printf("Awake called!\n");
		trans = GetComponent<TransformComponent>();
	}

	void Start() override
	{
		printf("Start called!\n");
	}

	void Update(const float deltaTime) override
	{
		lerpTime += deltaTime;
		if (lerpTime > 1.0f)
		{
			lerpTime = 0.0f;
		}
		const auto nextPosition = Vector3Lerp(fromPosition, toPosition, lerpTime);
		const auto nextRotation = Vector3Lerp(fromRotation, toRotation, lerpTime);
		trans->SetPosition(nextPosition);
		trans->SetRotation(QuaternionFromEuler(nextRotation.x, nextRotation.y, nextRotation.z));
		auto message = "Position: " + std::to_string(nextPosition.x) + ", " + std::to_string(nextPosition.y) + ", " + std::to_string(nextPosition.z);
		auto rotationMessage = "Rotation: " + std::to_string(nextRotation.x) + ", " + std::to_string(nextRotation.y) + ", " + std::to_string(nextRotation.z);
		TraceLog(LOG_INFO, rotationMessage.c_str());
	}

	void FixedUpdate() override
	{
	}

	void LateUpdate() override
	{
	}

	void OnEnable() override
	{
		printf("OnEnable called!\n");
	}

	void OnDisable() override
	{
		printf("OnDisable called!\n");
	}

	void OnDestroy() override
	{
		printf("OnDestroy called!\n");
	}
private:
	TransformComponent* trans = nullptr;
	Vector3 fromPosition = { 0,0,0 };
	Vector3 toPosition = { 0,0,10 };

	Vector3 fromRotation = { 0,0,0 };
	Vector3 toRotation = { 90, 0,0 };
	float lerpTime = 0.0f;
};

void DrawGrid();

int main()
{
	const int sceneWidth = 800;
	const int sceneHeight = 450;

	InitWindow(sceneWidth, sceneHeight, "GameObject Lifecycle Example");

	Camera camera = { 0 };
	camera.position = { 10.0f, 10.0f, 10.0f };
	camera.target = { 0.0f, 0.0f, 0.0f };
	camera.up = { 0.0f, 1.0f, 0.0f };
	camera.fovy = 45.0f;
	camera.projection = CAMERA_PERSPECTIVE;

	Model model = LoadModel("Assets/resources/rock.obj");

	model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = LoadTexture("Assets/resources/rock.png");

	const auto gameObject = std::make_shared<MyGameObject>();
	gameObject->GetComponent<TransformComponent>()->SetPosition({ 10,0,0 });
	gameObject->AddComponent(std::make_unique<RenderComponent>(model));

	Scene scene;
	scene.AddGameObject(gameObject);

	gameObject->Start();

	SetTargetFPS(60);

	while (!WindowShouldClose())
	{
		const float deltaTime = GetFrameTime();
		scene.Update(deltaTime);

		BeginDrawing();
		ClearBackground(RAYWHITE);
		BeginMode3D(camera);

		DrawGrid();
		scene.Draw();
		EndMode3D();
		EndDrawing();
	}

	scene.Unload();
	UnloadModel(model);
	CloseWindow();
	return 0;
}

void DrawGrid()
{
	DrawLine3D({ 0,0,0 }, { 10,0,0 }, RED);
	DrawLine3D({ 0,0,0 }, { 0,10,0 }, GREEN);
	DrawLine3D({ 0,0,0 }, { 0,0,10 }, BLUE);
}

