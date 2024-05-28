#include "raylib.h"
#include "InputHandler.h"
#include "vector"

using namespace command_pattern;

int main(void)
{
	InitWindow(800, 450, "raylib [core] example - basic window");

	InputHandler inputHandler;
	inputHandler.InitInput();
	const std::string name = "hong";
	auto actor = GameActor(name);
	const std::string name2 = "phuong";
	auto actor2 = GameActor(name2);
	std::vector<GameActor> actors;
	actors.push_back(actor);
	actors.push_back(actor2);

	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(RAYWHITE);
		Command* command = inputHandler.HandleInput();
		if (command)
		{
			for (GameActor a : actors)
			{
				command->Execute(a);
			}
		}

		DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);
		EndDrawing();
	}

	CloseWindow();

	return 0;
}