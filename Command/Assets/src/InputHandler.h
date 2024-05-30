#pragma once
#include "Command.h"
#include "MoveUnitCommand.h"
#include "Unit.h"
#include <vector>
using namespace CommandPattern;

class InputHandler
{
public:
	~InputHandler();
	void InitInput();
	Command* HandleInput();

private:
	unsigned int historyCapacity = 10;
	Command* buttonX = nullptr;
	Command* buttonY = nullptr;
	Command* buttonA = nullptr;
	Command* buttonB = nullptr;
	MoveUnitCommand* moveUnitCommand = nullptr;
	MoveUnitCommand* currentCommand = nullptr;
	std::vector<MoveUnitCommand*>* commandHistory = nullptr;
	Unit* unit = nullptr;

	void ClearReferenceCommand() const;
	void RemoveToEndHistory() const;
	int GetIndexOfCurrentCommand() const;
	MoveUnitCommand* GetCommandFromHistory(const int index) const;
};

