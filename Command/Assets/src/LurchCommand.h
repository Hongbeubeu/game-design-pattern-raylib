#pragma once
#include "Command.h"

namespace command_pattern
{
	class LurchCommand :public Command
	{
	public:
		virtual void Execute(GameActor& actor);
	};

}

