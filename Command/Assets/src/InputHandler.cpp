#include "raylib.h"

#include "InputHandler.h"
#include "FireCommand.h"
#include "LurchCommand.h"
#include "SwapCommand.h"
#include "JumpCommand.h"
#include "MoveUnitCommand.h"
#include "Unit.h"

using namespace CommandPattern;

InputHandler::~InputHandler()
{
	ClearReferenceCommand();
}

void InputHandler::ClearReferenceCommand() const
{
	delete buttonX;
	delete buttonY;
	delete buttonA;
	delete buttonB;
	delete moveUnitCommand;
	delete lastMoveUnitCommand;
	delete unit;
}

void InputHandler::InitInput()
{
	ClearReferenceCommand();
	buttonX = new SwapCommand();
	buttonY = new LurchCommand();
	buttonA = new JumpCommand();
	buttonB = new FireCommand();
	unit = new Unit();
	moveUnitCommand = new MoveUnitCommand(unit, 0, 0);
}

Command* InputHandler::HandleInput()
{
	if (IsKeyPressed(KEY_RIGHT))
	{
		return buttonX;
	}
	if (IsKeyPressed(KEY_LEFT))
	{
		return buttonY;
	}
	if (IsKeyPressed(KEY_UP))
	{
		return buttonA;
	}
	if (IsKeyPressed(KEY_DOWN))
	{
		return buttonB;
	}

	if (IsKeyPressed(KEY_W))
	{
		const int destY = unit->Y() + 1;
		moveUnitCommand->SetPosition(unit->X(), destY);
		lastMoveUnitCommand = moveUnitCommand;
		return moveUnitCommand;
	}

	if (IsKeyPressed(KEY_A))
	{
		const int destX = unit->X() - 1;
		moveUnitCommand->SetPosition(destX, unit->Y());
		lastMoveUnitCommand = moveUnitCommand;
		return moveUnitCommand;
	}

	if (IsKeyPressed(KEY_S))
	{
		const int destY = unit->Y() - 1;
		moveUnitCommand->SetPosition(unit->X(), destY);
		lastMoveUnitCommand = moveUnitCommand;
		return moveUnitCommand;
	}

	if (IsKeyPressed(KEY_D))
	{
		const int destX = unit->X() + 1;
		moveUnitCommand->SetPosition(destX, unit->Y());
		lastMoveUnitCommand = moveUnitCommand;
		return moveUnitCommand;
	}

	if (lastMoveUnitCommand && (IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL)) && IsKeyPressed(KEY_Z))
	{
		lastMoveUnitCommand->Undo();
		lastMoveUnitCommand = nullptr;
	}

	return nullptr;
}
