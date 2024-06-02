#pragma once
#include <memory>
#include <vector>

#include "Command.h"
#include "UnifiedCommandPool.h"

class CommandManager
{
public:
	CommandManager() = default;
	explicit CommandManager(const size_t maxHistorySize) : maxHistorySize(maxHistorySize) {}

	template <typename T>
	void ExecuteCommand(std::unique_ptr<T> command)
	{
		command->Execute();
		if (history.size() >= maxHistorySize)
		{
			auto cmd = std::move(history.front());
			history.erase(history.begin());
			ReturnCommand(std::move(cmd));
		}
		history.push_back(std::move(command));
		redoStack.clear();
	}
	void Undo();
	void Redo();

	template<typename T>
	std::unique_ptr<T> CreateCommand()
	{
		return pool.GetCommand<T>();
	}

	template <typename T>
	void ReturnCommand(std::unique_ptr<T> cmd)
	{
		pool.ReturnCommand(std::move(cmd));
	}

	void DrawInfo() const;

private:
	size_t maxHistorySize;
	std::vector<std::unique_ptr<CommandPattern::Command>> history;
	std::vector<std::unique_ptr<CommandPattern::Command>> redoStack;
	UnifiedCommandPool pool;
};

