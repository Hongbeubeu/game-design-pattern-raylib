#pragma once
#include "Command.h"

namespace CommandPattern
{
	class JumpCommand :public Command
	{
	public:
		virtual void Execute();
	};
}


