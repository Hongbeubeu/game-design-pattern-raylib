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
}

Command* InputHandler::HandleInput() const
{
	if (IsKeyPressed(KEY_RIGHT))
	{
		return buttonX;
	}
	else if (IsKeyPressed(KEY_LEFT))
	{
		return buttonY;
	}
	else if (IsKeyPressed(KEY_UP))
	{
		return buttonA;
	}
	else if (IsKeyPressed(KEY_DOWN))
	{
		return buttonB;
	}

	if (IsKeyPressed(KEY_W))
	{
		const int destY = unit->Y() + 1;
		unit->SetY(destY);
		return new MoveUnitCommand(unit, unit->X(), destY);
	}

	if (IsKeyPressed(KEY_S))
	{
		const int destY = unit->Y() - 1;
		unit->SetY(destY);
		return new MoveUnitCommand(unit, unit->X(), destY);
	}
	return nullptr;
}
