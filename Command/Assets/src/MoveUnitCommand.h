#pragma once
#include "Unit.h"
#include "Command.h"

namespace CommandPattern
{
	class MoveUnitCommand :public Command
	{
	public:
		MoveUnitCommand(Unit* unit, const int x, const int y) : unit(unit), x(x), y(y), prevX(0), prevY(0)
		{
		}

		virtual void Execute();
		virtual void Undo();
	private:
		Unit* unit;
		int x, y;
		int prevX, prevY;
	};
}


