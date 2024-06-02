#pragma once
#include "Command.h"
#include "Unit.h"

#include "CommandManager.h"
using namespace CommandPattern;

class InputHandler
{
public:
	~InputHandler() = default;
	void InitInput(std::shared_ptr<Unit> unit, std::unique_ptr<CommandManager> manager);
	void HandleInput();

private:
	std::shared_ptr<Unit> handledUnit = nullptr;
	std::unique_ptr<CommandManager> commandManager = nullptr;
};

