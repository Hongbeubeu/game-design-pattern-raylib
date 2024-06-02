#pragma once
#include "CommandPool.h"
#include <typeindex>
#include <unordered_map>

class UnifiedCommandPool
{
public:
	template<typename T>
	std::unique_ptr<T> GetCommand()
	{
		auto& pool = GetPool<T>();
		return pool.GetCommand();
	}

	template<typename T>
	void ReturnCommand(std::unique_ptr<T> cmd)
	{
		auto& pool = GetPool<T>();
		pool.ReturnCommand(std::move(cmd));
	}
private:
	template<typename T>
	CommandPool<T>& GetPool()
	{
		const auto typeIndex = std::type_index(typeid(T));
		if (pools.find(typeIndex) == pools.end())
		{
			pools[typeIndex] = std::make_unique<CommandPool<T>>();
		}
		return *static_cast<CommandPool<T>*>(pools[typeIndex].get());
	}

	std::unordered_map<std::type_index, std::unique_ptr<CommandPoolBase>> pools;
};

