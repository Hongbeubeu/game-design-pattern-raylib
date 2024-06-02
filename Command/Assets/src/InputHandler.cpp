#include "raylib.h"

#include "InputHandler.h"
#include "MoveUnitCommand.h"
#include "Unit.h"

using namespace CommandPattern;

InputHandler::~InputHandler()
{
	delete unit;
	delete commandManager;
}

void InputHandler::InitInput()
{
	unit = new Unit();
	commandManager = new CommandManager();
}
void InputHandler::HandleInput()
{
	int destX = unit->X();
	int destY = unit->Y();

	const auto keyPressed = GetKeyPressed();

	switch (keyPressed)
	{
	case KEY_W:
		destY += 1;
		break;
	case KEY_A:
		destX -= 1;
		break;
	case KEY_S:
		destY -= 1;
		break;
	case KEY_D:
		destX += 1;
		break;
	default:
		break;
	}

	if (destX != unit->X() || destY != unit->Y())
	{
		commandManager->ExecuteCommand(std::make_unique<MoveUnitCommand>(unit, destX, destY));
	}


	if ((IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL)) && IsKeyPressed(KEY_Z))
	{
		commandManager->Undo();
	}

	if ((IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL)) && IsKeyPressed(KEY_R))
	{
		commandManager->Redo();
	}
}
