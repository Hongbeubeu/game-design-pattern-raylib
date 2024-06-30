#include "Animator.h"
#include <iostream>
#include <cmath>
#include "AnimatorDataAsset.h"

#pragma region Public Methods

unsigned long long Animator::GetHash(const std::string& str)
{
	constexpr std::hash<std::string> hasher;
	return hasher(str);
}

void Animator::Load(const std::string& dataPath)
{
	auto data = AnimatorDataAssetLoader::Load(dataPath);
	for (auto& [key, value] : data.animations)
	{
		LoadAnimation(key, value);
	}

	for (auto& [fromAnimationKey, value] : data.transitions)
	{
		for (auto& [toAnimationKey, transitionData] : value)
		{
			LoadTransition(fromAnimationKey, toAnimationKey, transitionData);
		}
	}
}

void Animator::Unload() const
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
	for (const auto& [animationKey, transition] : transitions[currentState])
	{
		// Skip automatic transitions without conditions
		if (transition.automatic && transition.conditions.empty())
		{
			continue;
		}
		if (CheckConditions(transition.conditions))
		{
			ChangeState(animationKey);
			for (const auto& condition : transition.conditions)
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

#pragma endregion

#pragma region Private Methods
void Animator::LoadAnimation(const std::string& animationKey, const AnimationData& animationData)
{
	Animation anim;
	anim.frameTime = animationData.frameTime;
	anim.frameCount = animationData.frameCount;
	anim.spriteSheet = LoadTexture(animationData.spriteSheet.c_str());
	anim.frameWidth = anim.spriteSheet.width / anim.frameCount;
	anim.frameHeight = anim.spriteSheet.height;
	anim.origin = Vector2{ animationData.origin.x, animationData.origin.y };
	anim.loop = animationData.loop;
	anim.speedFactor = animationData.speedFactor;

	if (animationData.isDefault)
	{
		currentState = animationKey;
	}

	if (!animationData.triggers.empty())
	{
		for (const auto& [fst, snd] : animationData.triggers)
		{
			auto& frame = fst;
			const auto& trigger = snd;
			anim.triggers[std::stoi(frame)] = trigger;
		}
	}

	animations[animationKey] = anim;
}

void Animator::LoadTransition(const std::string& fromAnimationKey, const std::string& toAnimationKey, const TransitionData& transitionData)
{
	std::map<std::string, Transition> transitionMap;
	if (transitions.find(fromAnimationKey) != transitions.end())
	{
		transitionMap = transitions[fromAnimationKey];
	}

	Transition transition;
	transition.targetState = toAnimationKey;
	transition.blendDuration = transitionData.blendDuration;
	for (auto& conditionData : transitionData.conditions)
	{
		Condition condition;
		condition.parameterName = conditionData.parameter;

		std::string type = conditionData.type;
		if (type == "bool") condition.type = CONDITION_BOOL;
		else if (type == "float") condition.type = CONDITION_FLOAT;
		else if (type == "int") condition.type = CONDITION_INT;
		else if (type == "trigger") condition.type = CONDITION_TRIGGER;

		std::string comparison = conditionData.comparison;
		if (comparison == "equal") condition.comparison = COMPARE_EQUAL;
		else if (comparison == "not_equal") condition.comparison = COMPARE_NOT_EQUAL;
		else if (comparison == "greater") condition.comparison = COMPARE_GREATER;
		else if (comparison == "less") condition.comparison = COMPARE_LESS;
		else if (comparison == "greater_equal") condition.comparison = COMPARE_GREATER_EQUAL;
		else if (comparison == "less_equal") condition.comparison = COMPARE_LESS_EQUAL;

		if (condition.type == CONDITION_BOOL) condition.boolValue = conditionData.value;
		else if (condition.type == CONDITION_FLOAT) condition.floatValue = conditionData.value;
		else if (condition.type == CONDITION_INT) condition.intValue = conditionData.value;

		transition.conditions.push_back(condition);
	}
	transition.automatic = transitionData.automatic;
	transitionMap[transition.targetState] = transition;

	transitions[fromAnimationKey] = transitionMap;
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

bool Animator::CheckConditions(const std::vector<Condition>& conditions)
{
	for (const auto& condition : conditions)
	{
		if (parameters.find(condition.parameterName) == parameters.end() && condition.type != CONDITION_TRIGGER)
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

void Animator::ChangeState(const std::string& newState)
{
	if (currentState != newState && transitions[currentState].find(newState) != transitions[currentState].end())
	{
		auto timeToEndAnim = animations[currentState].frameTime * static_cast<float>(animations[currentState].frameCount) - static_cast<float>(currentFrame) * animations[currentState].frameTime;
		previousState = currentState;
		currentState = newState;
		blendDuration = transitions[previousState][newState].blendDuration >= timeToEndAnim ? timeToEndAnim : transitions[previousState][newState].blendDuration;
		timer = 0.0f;
		currentFrame = GetFirstFrame();
		if (blendDuration > 0.0f)
		{
			blending = true;
			blendTimer = 0.0f;
		}
	}
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

				// Check for automatic transitions
				if (const auto it = transitions.find(currentState); it != transitions.end())
				{
					for (const auto& [targetState, transition] : it->second)
					{
						if (transition.automatic && transition.conditions.empty())
						{
							ChangeState(targetState);
							return;
						}
					}
				}
			}
		}

		// Check if any triggers should be fired
		CheckTriggers();
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

void Animator::DoDrawTexture(const Animation& anim, const Vector2 position, const float scale, const float blendFactor) const
{
	const Rectangle source = { static_cast<float>(currentFrame * anim.frameWidth), 0,static_cast<float>(anim.frameWidth), static_cast<float>(anim.frameHeight) };
	const Rectangle dest = { position.x, position.y, static_cast<float>(anim.frameWidth) * scale, static_cast<float>(anim.frameHeight) * scale };
	const Color tint = Fade(WHITE, blendFactor);
	const auto origin = Vector2{ dest.width * anim.origin.x, dest.height * anim.origin.y };
	DrawTexturePro(anim.spriteSheet, source, dest, origin, 0.0f, tint);
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

#pragma endregion
