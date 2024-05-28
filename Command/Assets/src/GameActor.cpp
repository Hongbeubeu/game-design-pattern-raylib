#include "GameActor.h"

#include "raylib.h"

GameActor::GameActor(const std::string& name)
{
	this->name = name;
}

void GameActor::Lurch() const
{
	const std::string text = "Lurch! - " + name;
	TraceLog(LOG_INFO, text.c_str());
}

void GameActor::Fire() const
{
	const std::string text = "Fire! - " + name;
	TraceLog(LOG_INFO, text.c_str());
}

void GameActor::Jump() const
{
	const std::string text = "Jump! - " + name;
	TraceLog(LOG_INFO, text.c_str());
}

void GameActor::Swap() const
{
	const std::string text = "Swap! - " + name;
	TraceLog(LOG_INFO, text.c_str());
}
