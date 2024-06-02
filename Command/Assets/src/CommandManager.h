#pragma once
#include <memory>
#include <vector>

#include "Command.h"

class CommandManager
{
public:
	void ExecuteCommand(std::unique_ptr<CommandPattern::Command> command);
	void Undo();
	void Redo();
	void DrawInfo() const;

private:
	std::vector<std::unique_ptr<CommandPattern::Command>> history;
	std::vector<std::unique_ptr<CommandPattern::Command>> redoStack;
};

