#pragma once
#include "Command.h"
#include "MoveUnitCommand.h"
#include "Unit.h"
using namespace CommandPattern;

class InputHandler
{
public:
	~InputHandler();
	void InitInput();
	Command* HandleInput();

private:
	Command* buttonX = nullptr;
	Command* buttonY = nullptr;
	Command* buttonA = nullptr;
	Command* buttonB = nullptr;
	MoveUnitCommand* moveUnitCommand = nullptr;
	MoveUnitCommand* lastMoveUnitCommand = nullptr;
	Unit* unit = nullptr;

	void ClearReferenceCommand() const;
};

