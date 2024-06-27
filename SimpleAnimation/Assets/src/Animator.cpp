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
		blendTimer += deltaTime;
		if (blendTimer >= blendDuration)
		{
			blending = false;
			currentFrame = 0;
			timer = 0.0f;
		}
	}
	else
	{
		timer += deltaTime;
		if (timer >= animations[currentState].frameTime)
		{
			timer = 0.0f;
			currentFrame = (currentFrame + 1) % animations[currentState].frameCount;
			CheckTriggers();
		}
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
		currentFrame = 0;
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
		const Animation& anim = animations[currentState];
		const Rectangle source = { static_cast<float>(currentFrame * anim.frameWidth), 0,static_cast<float>(anim.frameWidth), static_cast<float>(anim.frameHeight) };
		const Rectangle dest = { position.x, position.y, static_cast<float>(anim.frameWidth) * scale, static_cast<float>(anim.frameHeight) * scale };
		const auto origin = Vector2{ dest.width * anim.origin.x, dest.height * anim.origin.y };
		DrawTexturePro(anim.spriteSheet, source, dest, origin, 0.0f, WHITE);
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
	const Rectangle sourcePrev = { static_cast<float>(framePrev * animPrev.frameWidth), 0, static_cast<float>(animPrev.frameWidth), static_cast<float>(animPrev.frameHeight) };
	const Rectangle destPrev = { position.x, position.y, static_cast<float>(animPrev.frameWidth) * scale, static_cast<float>(animPrev.frameHeight) * scale };
	const Color tintPrev = Fade(WHITE, 1.0f - blendFactor);
	auto origin = Vector2{ destPrev.width * animPrev.origin.x, destPrev.height * animPrev.origin.y };
	DrawTexturePro(animPrev.spriteSheet, sourcePrev, destPrev, origin, 0.0f, tintPrev);

	// Draw current animation frame
	const Rectangle sourceCurr = { static_cast<float>(frameCurr * animCurr.frameWidth), 0, static_cast<float>(animCurr.frameWidth), static_cast<float>(animCurr.frameHeight) };
	const Rectangle destCurr = { position.x, position.y, static_cast<float>(animCurr.frameWidth) * scale, static_cast<float>(animCurr.frameHeight) * scale };
	const Color tintCurr = Fade(WHITE, blendFactor);
	origin.x = destCurr.width * animCurr.origin.x;
	origin.y = destCurr.height * animCurr.origin.y;
	DrawTexturePro(animCurr.spriteSheet, sourceCurr, destCurr, origin, 0.0f, tintCurr);
}

void Animator::RegisterTriggerCallback(const std::string& trigger, const std::function<void()>& callback)
{
	triggerCallbacks[trigger] = callback;
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
