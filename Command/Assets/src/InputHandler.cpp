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
	delete commandHistory;
	delete unit;
}

void InputHandler::RemoveToEndHistory() const
{
	if (currentCommand == nullptr || commandHistory == nullptr || commandHistory->empty())
	{
		return;
	}

	auto it = std::find(commandHistory->begin(), commandHistory->end(), currentCommand);
	if (it == commandHistory->end())
	{
		return;
	}
	it = (it + 1);
	if (it != commandHistory->end())
	{
		commandHistory->erase(it, commandHistory->end());
	}
}

int InputHandler::GetIndexOfCurrentCommand() const
{
	if (commandHistory->empty() || currentCommand == nullptr)
	{
		return -1;
	}

	const auto it = std::find(commandHistory->begin(), commandHistory->end(), currentCommand);
	if (it != commandHistory->end())
	{
		return static_cast<int>(it - commandHistory->begin());
	}

	return -1;
}

MoveUnitCommand* InputHandler::GetCommandFromHistory(const int index) const
{
	if (index < 0 || index >= static_cast<int>(commandHistory->size()))
	{
		return nullptr;
	}

	return commandHistory->at(index);
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
	commandHistory = new std::vector<MoveUnitCommand*>();
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

	MoveUnitCommand* returnCommand = nullptr;

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
		returnCommand = new MoveUnitCommand(unit, destX, destY);
	}

	if (returnCommand)
	{
		RemoveToEndHistory();
		commandHistory->push_back(returnCommand);
		if (commandHistory->size() > historyCapacity)
		{
			commandHistory->erase(commandHistory->begin());
		}
		currentCommand = returnCommand;
	}

	if (!(commandHistory->empty()) && (IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL)) && IsKeyPressed(KEY_Z))
	{
		const auto index = GetIndexOfCurrentCommand();
		if (index > 0)
		{
			const auto undoCommand = GetCommandFromHistory(index - 1);
			currentCommand = undoCommand;
			returnCommand = undoCommand;
		}
	}

	if (!(commandHistory->empty()) && (IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL)) && IsKeyPressed(KEY_R))
	{
		const auto index = GetIndexOfCurrentCommand();
		if (index >= 0 && index < static_cast<int>(commandHistory->size()) - 1)
		{
			const auto redoCommand = GetCommandFromHistory(index + 1);
			currentCommand = redoCommand;
			returnCommand = redoCommand;
		}
	}

	return returnCommand;
}
