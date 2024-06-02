#pragma once
#include <memory>

#include "Unit.h"
#include "Command.h"

namespace CommandPattern
{
	class MoveUnitCommand :public Command
	{
	public:
		MoveUnitCommand(std::shared_ptr<Unit> unit, const int x, const int y) : unit(std::move(unit)), x(x), y(y), prevX(0), prevY(0)
		{
		}

		virtual void Execute();
		virtual void Undo();
	private:
		std::shared_ptr<Unit> unit;
		int x, y;
		int prevX, prevY;
	};
}


