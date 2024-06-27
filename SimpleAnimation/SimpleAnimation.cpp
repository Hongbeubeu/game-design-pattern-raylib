#include <iostream>
#include "Animator.h"
#include "raylib.h"

int main()
{
	// Initialization
	constexpr int screenWidth = 800;
	constexpr int screenHeight = 450;

	InitWindow(screenWidth, screenHeight, "2D Animation with Raylib");
	// Load your icon image
	const Image icon = LoadImage("Assets/resources/icons/icon.png"); // Make sure to provide the correct path

	// Set the loaded image as the window icon
	SetWindowIcon(icon);


	Animator animator;
	animator.LoadAnimations("Assets/resources/datas/animation.json");

	// Register trigger callbacks
	animator.RegisterTriggerCallback("play_sound", []()
		{
			std::cout << "Playing sound!\n";
		});

	animator.RegisterTriggerCallback("deal_damage", []()
		{
			std::cout << "Dealing damage!\n";
		});

	SetTargetFPS(60);

	while (!WindowShouldClose())
	{
		const float deltaTime = GetFrameTime();

		// Simulate changing states
		if (IsKeyPressed(KEY_Q))
		{
			animator.ChangeState("idle");
		}
		else if (IsKeyPressed(KEY_W))
		{
			animator.ChangeState("attack");
		}
		else if (IsKeyPressed(KEY_E))
		{
			animator.ChangeState("dead");
		}
		else if (IsKeyPressed(KEY_R))
		{
			animator.ChangeState("hit");
		}
		else if (IsKeyPressed(KEY_T))
		{
			animator.ChangeState("react");
		}
		else if (IsKeyPressed(KEY_Y))
		{
			animator.ChangeState("walk");
		}

		animator.Update(deltaTime);

		BeginDrawing();
		ClearBackground(BLUE);

		animator.Draw(Vector2{ screenWidth / 2.0f, screenHeight / 2.0f }, 5.0f);
		EndDrawing();
	}
	animator.UnloadAnimations();
	CloseWindow();

	return 0;
}
