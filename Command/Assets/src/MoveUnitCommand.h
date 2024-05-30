#pragma once
#include "Unit.h"
#include "Command.h"

namespace CommandPattern
{
	class MoveUnitCommand :public Command
	{
	public:
		MoveUnitCommand(Unit* unit, const int x, const int y) : unit(unit), x(x), y(y) {}
		virtual void Execute();
	private:
		Unit* unit;
		int x, y;
	};
}


