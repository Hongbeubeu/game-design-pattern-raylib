#include "raylib.h"
#include "InputHandler.h"

using namespace CommandPattern;

int main(void)
{
	InitWindow(800, 450, "raylib [core] example - basic window");

	InputHandler inputHandler;
	inputHandler.InitInput();
	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(RAYWHITE);
		inputHandler.HandleInput();
		DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);
		EndDrawing();
	}

	CloseWindow();

	return 0;
}