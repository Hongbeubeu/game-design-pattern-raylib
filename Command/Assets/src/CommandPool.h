#pragma once
#include <memory>
#include <queue>
#include "CommandPoolBase.h"

template <typename T>
class CommandPool : public CommandPoolBase
{
public:
	std::unique_ptr<T> GetCommand()
	{
		if (!pool.empty())
		{
			auto cmd = std::move(pool.front());
			pool.pop();
			cmd->Reset();
			return cmd;
		}
		return std::make_unique<T>();
	}

	void ReturnCommand(std::unique_ptr<T> cmd)
	{
		pool.push(std::move(cmd));
	}

private:
	std::queue<std::unique_ptr<T>> pool;
};

