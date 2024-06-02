#pragma once
#include "Command.h"
#include "Unit.h"

#include "CommandManager.h"
using namespace CommandPattern;

class InputHandler
{
public:
	~InputHandler() = default;
	void InitInput(std::shared_ptr<Unit> unit);
	void HandleInput();

private:
	std::shared_ptr<Unit> handledUnit = nullptr;
	CommandManager commandManager = CommandManager(20);
};

