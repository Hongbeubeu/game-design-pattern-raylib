#pragma once
#include "Command.h"
using namespace CommandPattern;

class InputHandler
{
public:
	~InputHandler();
	void InitInput();
	Command* HandleInput() const;

private:
	Command* buttonX = nullptr;
	Command* buttonY = nullptr;
	Command* buttonA = nullptr;
	Command* buttonB = nullptr;

	void ClearReferenceCommand() const;
};

