#include "MoveUnitCommand.h"

#include "raylib.h"

namespace CommandPattern
{
	void MoveUnitCommand::Init(std::shared_ptr<Unit> unit, const int x, const int y)
	{
		this->handledUnit = std::move(unit);
		this->currentX = x;
		this->currentY = y;
	}

	void MoveUnitCommand::Execute()
	{
		prevX = handledUnit->X();
		prevY = handledUnit->Y();
		handledUnit->MoveTo(currentX, currentY);
	}

	void MoveUnitCommand::Undo()
	{
		handledUnit->MoveTo(prevX, prevY);
	}

	void MoveUnitCommand::Reset()
	{
		currentX = 0;
		currentY = 0;
		prevX = 0;
		prevY = 0;
		handledUnit.reset();
	}

	void MoveUnitCommand::DrawInfo(const Color color)
	{
		DrawRectangle(currentX, currentY, 32, 32, color);
	}
}

