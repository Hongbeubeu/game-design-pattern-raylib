#pragma once
#include "GameActor.h"

namespace CommandPattern
{
	class Command
	{
	public:
		virtual ~Command() = default;
		virtual void Execute() = 0;
	};
}

