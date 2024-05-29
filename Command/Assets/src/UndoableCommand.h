#pragma once
#include "Command.h"

namespace CommandPattern
{
	class UndoableCommand :public Command
	{
	public:
		virtual void Undo() = 0;
	};
}

