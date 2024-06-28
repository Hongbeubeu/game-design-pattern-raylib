#include "Animator.h"
#include <iostream>
#include <fstream>
#include <cmath>

void Animator::LoadAnimations(const std::string& configFile)
{
	std::ifstream file(configFile);
	if (file.is_open())
	{
		nlohmann::json jsonData;
		file >> jsonData;

		for (auto& item : jsonData["animations"].items())
		{
			auto& key = item.key();
			auto& value = item.value();
			Animation anim;
			anim.frameCount = value["frame_count"];
			anim.spriteSheet = LoadTexture(value["sprite_sheet"].get<std::string>().c_str());
			anim.frameTime = value["frame_time"];
			anim.frameWidth = anim.spriteSheet.width / anim.frameCount;
			anim.frameHeight = anim.spriteSheet.height;
			anim.origin = Vector2{
				value["origin"]["x"],
				value["origin"]["y"]
			};
			anim.loop = value["loop"];
			anim.speedFactor = value["speed_factor"];

			if (value["default"])
			{
				currentState = key;
			}

			if (value.contains("triggers"))
			{
				for (auto& triggerItems : value["triggers"].items())
				{
					auto& frame = triggerItems.key();
					const auto& triggers = triggerItems.value().get<std::vector<std::string>>();
					anim.triggers[std::stoi(frame)] = triggers;
				}
			}

			animations[key] = anim;
		}

		for (auto& transitionItems : jsonData["transitions"].items())
		{
			auto& key = transitionItems.key();
			auto& value = transitionItems.value();

			std::map<std::string, Transition> transitionMap;

			for (auto& transitionItem : value.items())
			{
				Transition transition;
				transition.targetState = transitionItem.key();
				transition.blendDuration = transitionItem.value()["blend_duration"];
				for (auto& conditionData : transitionItem.value()["conditions"])
				{
					Condition condition;
					condition.parameterName = conditionData["parameter"];
					std::string type = conditionData["type"];
					if (type == "bool") condition.type = CONDITION_BOOL;
					else if (type == "float") condition.type = CONDITION_FLOAT;
					else if (type == "int") condition.type = CONDITION_INT;
					else if (type == "trigger") condition.type = CONDITION_TRIGGER;

					std::string comparison = conditionData["comparison"];
					if (comparison == "equal") condition.comparison = COMPARE_EQUAL;
					else if (comparison == "not_equal") condition.comparison = COMPARE_NOT_EQUAL;
					else if (comparison == "greater") condition.comparison = COMPARE_GREATER;
					else if (comparison == "less") condition.comparison = COMPARE_LESS;
					else if (comparison == "greater_equal") condition.comparison = COMPARE_GREATER_EQUAL;
					else if (comparison == "less_equal") condition.comparison = COMPARE_LESS_EQUAL;

					if (condition.type == CONDITION_BOOL) condition.boolValue = conditionData["value"];
					else if (condition.type == CONDITION_FLOAT) condition.floatValue = conditionData["value"];
					else if (condition.type == CONDITION_INT) condition.intValue = conditionData["value"];

					transition.conditions.push_back(condition);
				}
				transitionMap[transition.targetState] = transition;
			}
			transitions[key] = transitionMap;
		}
	}
	else
	{
		std::cerr << "Failed to open file: " << configFile << '\n';
	}
}

void Animator::UnloadAnimations() const
{
	for (const auto& item : animations)
	{
		UnloadTexture(item.second.spriteSheet);
	}
}



void Animator::Update(const float deltaTime)
{
	if (blending)
	{
		DoBlending(deltaTime);
	}
	else
	{
		DoUpdate(deltaTime);
	}

	// Check if any transitions should occur based on conditions
	for (const auto& transition : transitions[currentState])
	{
		if (CheckConditions(transition.second.conditions))
		{
			ChangeState(transition.first);
			for (const auto& condition : transition.second.conditions)
			{
				if (condition.type == CONDITION_TRIGGER)
				{
					ResetTrigger(condition.parameterName);
				}
			}
			break;
		}
	}
}

void Animator::DoBlending(const float deltaTime)
{
	blendTimer += deltaTime;
	if (blendTimer >= blendDuration)
	{
		blending = false;
		currentFrame = GetFirstFrame();
		timer = 0.0f;
		CheckTriggers();
	}
}

int Animator::GetFirstFrame()
{
	const Animation& anim = animations[currentState];
	if (anim.speedFactor > 0)
	{
		return 0;
	}
	return anim.frameCount - 1;
}

int Animator::GetLastFrame()
{
	const Animation& anim = animations[currentState];
	if (anim.speedFactor > 0)
	{
		return anim.frameCount - 1;
	}
	return 0;
}

void Animator::DoUpdate(const float deltaTime)
{
	const Animation& anim = animations[currentState];

	// Update timer, wrapping around using std::fmod precision
	timer = std::fmod(timer + deltaTime * std::abs(anim.speedFactor), anim.frameTime);

	// Determine if we need to change the current frame based on the timer
	if (timer < deltaTime * std::abs(anim.speedFactor))
	{
		// Adjust current frame based on the direction of the animation
		currentFrame += (anim.speedFactor > 0) ? 1 : -1;

		// Handle animation looping or resetting to the first frame
		if (currentFrame >= anim.frameCount || currentFrame < 0)
		{
			if (anim.loop)
			{
				currentFrame = GetFirstFrame();
			}
			else
			{
				currentFrame = GetLastFrame();
			}
		}

		// Check if any triggers should be fired
		CheckTriggers();
	}
}

void Animator::ChangeState(const std::string& newState)
{
	if (currentState != newState && transitions[currentState].find(newState) != transitions[currentState].end())
	{
		previousState = currentState;
		currentState = newState;
		blendDuration = transitions[previousState][newState].blendDuration;
		timer = 0.0f;
		currentFrame = GetFirstFrame();
		if (blendDuration > 0.0f)
		{
			blending = true;
			blendTimer = 0.0f;
		}
	}
}


void Animator::Draw(const Vector2 position, const float scale)
{
	DrawText(currentState.c_str(), static_cast<int>(position.x), 0, 40, BLACK);
	if (blending)
	{
		DrawBlended(position, scale);
	}
	else
	{
		DoDrawTexture(animations[currentState], position, scale, 1);
	}
}

void Animator::DoDrawTexture(const Animation& anim, const Vector2 position, const float scale, const float blendFactor) const
{
	const Rectangle source = { static_cast<float>(currentFrame * anim.frameWidth), 0,static_cast<float>(anim.frameWidth), static_cast<float>(anim.frameHeight) };
	const Rectangle dest = { position.x, position.y, static_cast<float>(anim.frameWidth) * scale, static_cast<float>(anim.frameHeight) * scale };
	const Color tint = Fade(WHITE, blendFactor);
	const auto origin = Vector2{ dest.width * anim.origin.x, dest.height * anim.origin.y };
	DrawTexturePro(anim.spriteSheet, source, dest, origin, 0.0f, tint);
}

void Animator::DrawBlended(const Vector2 position, const float scale)
{
	const Animation& animPrev = animations[previousState];
	const Animation& animCurr = animations[currentState];

	const float blendFactor = blendTimer / blendDuration;

	// Calculate current frame for both animations
	const int framePrev = previousFrame + static_cast<int>(static_cast<float>(currentFrame - previousFrame) * blendFactor);
	const int frameCurr = currentFrame;

	// Draw previous animation frame
	DoDrawTexture(animPrev, position, scale, 1.0f - blendFactor);

	// Draw current animation frame
	DoDrawTexture(animCurr, position, scale, blendFactor);
}

void Animator::RegisterTriggerCallback(const std::string& trigger, const std::function<void()>& callback)
{
	triggerCallbacks[trigger] = callback;
}

void Animator::SetBool(const std::string& parameter, bool value)
{
	parameters[parameter] = value;
}

void Animator::SetInt(const std::string& parameter, int value)
{
	parameters[parameter] = value;
}

void Animator::SetFloat(const std::string& parameter, float value)
{
	parameters[parameter] = value;
}

void Animator::SetTrigger(const std::string& parameter)
{
	triggers[parameter] = true;
}

void Animator::ResetTrigger(const std::string& parameter)
{
	triggers[parameter] = false;
}

bool Animator::CheckConditions(const std::vector<Condition>& conditions)
{
	for (const auto& condition : conditions)
	{
		if (parameters.find(condition.parameterName) == parameters.end())
		{
			return false;
		}

		switch (condition.type)
		{
		case CONDITION_BOOL:
		{
			const bool paramValue = std::get<bool>(parameters[condition.parameterName]);
			if (condition.comparison == COMPARE_EQUAL && paramValue != condition.boolValue) return false;
			if (condition.comparison == COMPARE_NOT_EQUAL && paramValue == condition.boolValue) return false;
			break;
		}
		case CONDITION_INT:
		{
			const int paramValue = std::get<int>(parameters[condition.parameterName]);
			if (condition.comparison == COMPARE_EQUAL && paramValue != condition.intValue) return false;
			if (condition.comparison == COMPARE_NOT_EQUAL && paramValue == condition.intValue) return false;
			if (condition.comparison == COMPARE_GREATER && paramValue <= condition.intValue) return false;
			if (condition.comparison == COMPARE_LESS && paramValue >= condition.intValue) return false;
			if (condition.comparison == COMPARE_GREATER_EQUAL && paramValue < condition.intValue) return false;
			if (condition.comparison == COMPARE_LESS_EQUAL && paramValue > condition.intValue) return false;
			break;
		}
		case CONDITION_FLOAT:
		{
			const float paramValue = std::get<float>(parameters[condition.parameterName]);
			if (condition.comparison == COMPARE_EQUAL && paramValue != condition.floatValue) return false;
			if (condition.comparison == COMPARE_NOT_EQUAL && paramValue == condition.floatValue) return false;
			if (condition.comparison == COMPARE_GREATER && paramValue <= condition.floatValue) return false;
			if (condition.comparison == COMPARE_LESS && paramValue >= condition.floatValue) return false;
			if (condition.comparison == COMPARE_GREATER_EQUAL && paramValue < condition.floatValue) return false;
			if (condition.comparison == COMPARE_LESS_EQUAL && paramValue > condition.floatValue) return false;
			break;
		}
		case CONDITION_TRIGGER:
		{
			if (triggers.find(condition.parameterName) == triggers.end() || !triggers[condition.parameterName])
			{
				return false;
			}
			break;
		}
		}
	}
	return true;
}

void Animator::CheckTriggers()
{
	Animation& anim = animations[currentState];
	if (anim.triggers.find(currentFrame) != anim.triggers.end())
	{
		const std::vector<std::string> triggers = anim.triggers[currentFrame];
		for (const std::string& trigger : triggers)
			if (triggerCallbacks.find(trigger) != triggerCallbacks.end())
			{
				triggerCallbacks[trigger]();
			}
	}
}
