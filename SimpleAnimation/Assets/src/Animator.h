#pragma once
#include "raylib.h"
#include "json.hpp"

using json = nlohmann::json;

// Strut to hold the animation data
struct Animation
{
	int frameCount;
	Texture2D spriteSheet;
	int frameWidth;
	int frameHeight;
	Vector2 origin;
	float frameTime; // Time each frame is displayed
	std::map<int, std::vector<std::string>> triggers; // Map of frame index to trigger action
};

struct Transition
{
	std::string targetState;
	float blendDuration;
};


class Animator
{
public:
	void LoadAnimations(const std::string& configFile);
	void UnloadAnimations() const;

	void Update(float deltaTime);
	void ChangeState(const std::string& newState);
	void Draw(Vector2 position, float scale);
	void RegisterTriggerCallback(const std::string& trigger, const std::function<void()>& callback);
private:
	std::map<std::string, Animation> animations;
	std::map<std::string, std::map<std::string, Transition>> transitions;
	std::map<std::string, std::function<void()>> triggerCallbacks;
	std::string currentState;
	std::string previousState;
	int currentFrame = 0;
	int previousFrame = 0;
	float timer = 0;
	float blendTimer = 0;
	float blendDuration = 0;
	bool blending = false;

	void CheckTriggers();
	void DrawBlended(Vector2 position, float scale);
};
