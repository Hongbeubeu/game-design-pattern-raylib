#include "raylib.h"

#include "InputHandler.h"
#include "MoveUnitCommand.h"
#include "Unit.h"

using namespace CommandPattern;

void InputHandler::InitInput(std::shared_ptr<Unit> unit, std::unique_ptr<CommandManager> manager)
{
	this->handledUnit = std::move(unit);
	this->commandManager = std::move(manager);
}

void InputHandler::HandleInput()
{
	int destX = handledUnit->X();
	int destY = handledUnit->Y();

	const auto keyPressed = GetKeyPressed();

	switch (keyPressed)
	{
	case KEY_W:
		destY -= 32;
		break;
	case KEY_A:
		destX -= 32;
		break;
	case KEY_S:
		destY += 32;
		break;
	case KEY_D:
		destX += 32;
		break;
	default:
		break;
	}

	if (destX != handledUnit->X() || destY != handledUnit->Y())
	{
		commandManager->ExecuteCommand(std::make_unique<MoveUnitCommand>(handledUnit, destX, destY));
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
