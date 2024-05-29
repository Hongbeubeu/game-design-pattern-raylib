#pragma once
#include "Command.h"

namespace CommandPattern
{
	class LurchCommand :public Command
	{
	public:
		virtual void Execute();
	};

}

