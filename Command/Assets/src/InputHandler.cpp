#include "raylib.h"

#include "InputHandler.h"
#include "FireCommand.h"
#include "LurchCommand.h"
#include "SwapCommand.h"
#include "JumpCommand.h"

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
}

void InputHandler::InitInput()
{
	ClearReferenceCommand();
	buttonX = new SwapCommand();
	buttonY = new LurchCommand();
	buttonA = new JumpCommand();
	buttonB = new FireCommand();
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

	return nullptr;
}
