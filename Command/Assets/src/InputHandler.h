#pragma once
#include "Command.h"
#include "Unit.h"

#include "CommandManager.h"
using namespace CommandPattern;

class InputHandler
{
public:
	~InputHandler();
	void InitInput();
	void HandleInput();

private:
	Unit* unit = nullptr;
	CommandManager* commandManager = nullptr;
};

