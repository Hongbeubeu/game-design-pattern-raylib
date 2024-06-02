#include "CommandManager.h"

#include "MoveUnitCommand.h"
#include "raylib.h"

void CommandManager::ExecuteCommand(std::unique_ptr<CommandPattern::Command> command)
{
	command->Execute();
	history.push_back(std::move(command));
	redoStack.clear();
}

void CommandManager::Undo()
{
	if (!history.empty())
	{
		history.back()->Undo();
		redoStack.push_back(std::move(history.back()));
		history.pop_back();
	}
}

void CommandManager::Redo()
{
	if(!redoStack.empty())
	{
		redoStack.back()->Execute();
		history.push_back(std::move(redoStack.back()));
		redoStack.pop_back();
	}
}