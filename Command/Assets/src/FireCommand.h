#pragma once
#include "Command.h"

namespace CommandPattern
{
	class FireCommand :public Command
	{
	public:
		virtual void Execute();
	};
}


