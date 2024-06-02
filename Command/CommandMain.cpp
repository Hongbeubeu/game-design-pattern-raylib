#include "raylib.h"
#include "InputHandler.h"

using namespace CommandPattern;
constexpr int SCREEN_WIDTH = 800;
constexpr int SCREEN_HEIGHT = 450;
constexpr int CELL_SIZE = 32;

void DrawGrid()
{
	for (int i = 0; i < SCREEN_HEIGHT / CELL_SIZE; ++i)
	{
		DrawLine(0, i * CELL_SIZE, SCREEN_WIDTH, i * CELL_SIZE, GREEN);
	}
	for (int i = 0; i < SCREEN_WIDTH / CELL_SIZE; ++i)
	{
		DrawLine(i * CELL_SIZE, 0, i * CELL_SIZE, SCREEN_HEIGHT, GREEN);
	}
}

int main(void)
{
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Command Pattern");

	InputHandler inputHandler;
	const auto unit = std::make_shared<Unit>();
	auto manager = std::make_unique<CommandManager>();
	inputHandler.InitInput(unit, std::move(manager));

	// Main game loop
	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(RAYWHITE);
		DrawGrid();
		inputHandler.HandleInput();
		unit->Update();
		DrawText("Congrats! Welcome to my commands", 190, 200, 20, LIGHTGRAY);
		EndDrawing();
	}

	CloseWindow();

	return 0;
}
