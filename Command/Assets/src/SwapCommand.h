#pragma once
#include "Command.h"

namespace command_pattern
{
	class SwapCommand :public Command
	{
	public:
		virtual void Execute(GameActor& actor);
	};

}

