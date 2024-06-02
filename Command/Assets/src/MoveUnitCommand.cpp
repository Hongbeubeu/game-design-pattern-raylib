#include "MoveUnitCommand.h"

#include "raylib.h"

namespace CommandPattern
{
	void MoveUnitCommand::Execute()
	{
		prevX = unit->X();
		prevY = unit->Y();
		unit->MoveTo(x, y);
	}

	void MoveUnitCommand::Undo()
	{
		unit->MoveTo(prevX, prevY);
	}

	void MoveUnitCommand::DrawInfo(const Color color)
	{
		DrawRectangle(x, y, 32, 32, color);
	}
}

