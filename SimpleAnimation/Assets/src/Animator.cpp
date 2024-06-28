#include "Animator.h"
#include <iostream>
#include <fstream>

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
				transition.conditions = transitionItem.value()["conditions"].get<std::vector<std::string>>();
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
	else
	{
		return anim.frameCount - 1;
	}
}

void Animator::DoUpdate(const float deltaTime)
{
	const Animation& anim = animations[currentState];

	timer += deltaTime * abs(anim.speedFactor);
	if (timer >= anim.frameTime)
	{
		timer = 0.0f;
		if (anim.speedFactor > 0)
		{
			currentFrame++;
			if (currentFrame >= anim.frameCount)
			{
				if (anim.loop)
				{
					currentFrame = 0;
				}
				else
				{
					currentFrame = anim.frameCount - 1;
				}
			}
		}
		else
		{
			currentFrame--;
			if (currentFrame < 0)
			{
				if (anim.loop)
				{
					currentFrame = anim.frameCount - 1;
				}
				else
				{
					currentFrame = 0;
				}
			}
		}
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

void Animator::SetCondition(const std::string& condition, bool value)
{
	conditions[condition] = value;
}

bool Animator::CheckConditions(const std::vector<std::string>& requireConditions)
{
	for (const auto& condition : requireConditions)
	{
		if (conditions.find(condition) == conditions.end() || !conditions[condition])
		{
			return false;
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
