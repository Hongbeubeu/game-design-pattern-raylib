#include "raylib.h"
#include "InputHandler.h"
#include "vector"

using namespace CommandPattern;

int main(void)
{
	InitWindow(800, 450, "raylib [core] example - basic window");

	InputHandler inputHandler;
	inputHandler.InitInput();
	Command* command = nullptr;
	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(RAYWHITE);
		command = inputHandler.HandleInput();
		if (command)
		{
			command->Execute();
		}
		DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);
		EndDrawing();
	}
	delete command;
	CloseWindow();

	return 0;
}