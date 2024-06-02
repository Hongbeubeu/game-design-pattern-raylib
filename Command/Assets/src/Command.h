#pragma once
#include "raylib.h"

namespace CommandPattern
{
	// Command interface
	class Command
	{
	public:
		virtual ~Command() = default;
		virtual void Execute() = 0;
		virtual void Undo() = 0;
		virtual void DrawInfo(const Color color) = 0;
	};
}

