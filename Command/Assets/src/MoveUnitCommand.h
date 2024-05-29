#pragma once
#include "UndoableCommand.h"
#include "Unit.h"

namespace CommandPattern
{
	class MoveUnitCommand :public UndoableCommand
	{
	public:
		MoveUnitCommand(Unit* unit, const int x, const int y) : unit(unit), x(x), y(y), xBefore(0), yBefore(0) {}
		virtual void Execute();
		virtual void Undo();
		virtual void SetPosition(const int xValue, const int yValue);
	private:
		Unit* unit;
		int x, y;
		int xBefore, yBefore;
	};
}


