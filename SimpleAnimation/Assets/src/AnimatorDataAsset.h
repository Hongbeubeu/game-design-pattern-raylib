#pragma once
#include <string>
#include <fstream>
#include <iostream>

#include "json.hpp"

struct ConditionData
{
	std::string parameter;
	std::string type;
	std::string comparison;
	nlohmann::json value;
};

struct TransitionData
{
	float blendDuration;
	std::vector<ConditionData> conditions;
	bool automatic;
};

struct OriginData
{
	float x;
	float y;
};

struct AnimationData
{
	float frameTime;
	int frameCount;
	std::string spriteSheet;
	bool isDefault;
	OriginData origin;
	bool loop;
	float speedFactor;
	std::map<std::string, std::vector<std::string>> triggers;
};

struct AnimatorData
{
	std::map<std::string, AnimationData> animations;
	std::map<std::string, std::map<std::string, TransitionData>> transitions;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ConditionData, parameter, type, comparison, value)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(TransitionData, blendDuration, conditions, automatic)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(OriginData, x, y)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(AnimationData, frameTime, frameCount, spriteSheet, isDefault, origin, loop, speedFactor, triggers)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(AnimatorData, animations, transitions)


class AnimatorDataAssetLoader
{
public:
	static AnimatorData Load(const std::string& path)
	{
		std::ifstream file("Assets/resources/datas/animation.json");
		if (!file.is_open())
		{
			std::cerr << "Failed to open file!\n";
			return {};
		}

		nlohmann::json j;
		file >> j;
		const auto animatorData = j.get<AnimatorData>();
		return animatorData;
	}
};