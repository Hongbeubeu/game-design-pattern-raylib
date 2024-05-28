#pragma once
#include "Command.h"

namespace command_pattern
{
	class FireCommand :public Command
	{
	public:
		virtual void Execute(GameActor& actor);
	};
}


