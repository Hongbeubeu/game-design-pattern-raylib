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
	bool loop; // Whether the animation loops
	float speedFactor; // Speed multiplier
	std::map<int, std::vector<std::string>> triggers; // Map of frame index to trigger action
};


enum ConditionType
{
	CONDITION_BOOL,
	CONDITION_INT,
	CONDITION_FLOAT,
	CONDITION_TRIGGER
};

enum ComparisonType
{
	COMPARE_EQUAL,
	COMPARE_NOT_EQUAL,
	COMPARE_GREATER,
	COMPARE_LESS,
	COMPARE_GREATER_EQUAL,
	COMPARE_LESS_EQUAL
};

struct Condition
{
	std::string parameterName;
	ConditionType type;
	ComparisonType comparison;
	union 
	{
		bool boolValue;
		float floatValue;
		int intValue;
	};
};

struct Transition
{
	std::string targetState;
	float blendDuration;
	std::vector<std::string> conditions;
};

class Animator
{
public:
	void LoadAnimations(const std::string& configFile);
	void UnloadAnimations() const;
	void DoUpdate(float deltaTime);

	void Update(float deltaTime);
	void ChangeState(const std::string& newState);
	void Draw(Vector2 position, float scale);
	void RegisterTriggerCallback(const std::string& trigger, const std::function<void()>& callback);
	void SetBool(const std::string& parameter, bool value);
	void SetInt(const std::string& parameter, int value);
	void SetFloat(const std::string& parameter, float value);
	void SetTrigger(const std::string& parameter);
	void ResetTrigger(const std::string& parameter);
	void SetCondition(const std::string& condition, bool value);
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
	std::map<std::string, bool> conditions;

	void CheckTriggers();
	bool CheckConditions(const std::vector<std::string>& conditions);
	void DrawBlended(Vector2 position, float scale);
	void DoBlending(float deltaTime);
	void DoDrawTexture(const Animation& anim,Vector2 position, float scale, float blendFactor) const;
	int GetFirstFrame();
};
