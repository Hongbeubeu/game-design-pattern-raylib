#pragma once
#include "Command.h"

namespace command_pattern
{
	class JumpCommand :public Command
	{
	public:
		virtual void Execute(GameActor& actor);
	};
}


