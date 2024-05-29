#pragma once
#include "Command.h"

namespace CommandPattern
{
	class SwapCommand :public Command
	{
	public:
		virtual void Execute();
	};

}

