#pragma once
#include <memory>

#include "Unit.h"
#include "Command.h"

namespace CommandPattern
{
	class MoveUnitCommand :public Command
	{
	public:
		MoveUnitCommand() = default;
		MoveUnitCommand(std::shared_ptr<Unit> unit, const int x, const int y) : handledUnit(std::move(unit)), currentX(x), currentY(y), prevX(0), prevY(0)
		{
		}
		virtual void Init(std::shared_ptr<Unit> unit, const int x, const int y);
		virtual void Execute();
		virtual void Undo();
		virtual void Reset();
		virtual void DrawInfo(const Color color);
	private:
		std::shared_ptr<Unit> handledUnit;
		int currentX, currentY;
		int prevX, prevY;
	};
}


