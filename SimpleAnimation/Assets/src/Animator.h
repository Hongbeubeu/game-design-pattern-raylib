#pragma once
#include "raylib.h"
#include "json.hpp"
#include <variant>
#include <functional>

#include "AnimatorDataAsset.h"

using json = nlohmann::json;

// Strut to hold the animation data
struct Animation
{
	float frameTime; // Time each frame is displayed
	int frameCount;
	Texture2D spriteSheet;
	Vector2 origin;
	bool loop; // Whether the animation loops
	float speedFactor; // Speed multiplier
	std::map<int, std::vector<std::string>> triggers; // Map of frame index to trigger action
	int frameWidth;
	int frameHeight;
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
	std::vector<Condition> conditions;
	bool automatic; // 
};

class Animator
{
public:
	static unsigned long long GetHash(const std::string& str);

	void Load(const std::string& dataPath);
	void Unload() const;
	void Update(float deltaTime);
	void Draw(Vector2 position, float scale);
	void RegisterTriggerCallback(const std::string& trigger, const std::function<void()>& callback);
	void SetBool(const std::string& parameter, bool value);
	void SetInt(const std::string& parameter, int value);
	void SetFloat(const std::string& parameter, float value);
	void SetTrigger(const std::string& parameter);
	void ResetTrigger(const std::string& parameter);
private:
	std::map<std::string, Animation> animations;
	std::map<std::string, std::map<std::string, Transition>> transitions;
	std::map<std::string, std::variant<bool, float, int>> parameters;
	std::map<std::string, bool> triggers;
	std::map<std::string, std::function<void()>> triggerCallbacks;

	std::string currentState;
	std::string previousState;
	int currentFrame = 0;
	int previousFrame = 0;
	float timer = 0;
	float blendTimer = 0;
	float blendDuration = 0;
	bool blending = false;


	void LoadAnimation(const std::string& animationKey, const AnimationData& animationData);
	void LoadTransition(const std::string& fromAnimationKey, const std::string& toAnimationKey, const TransitionData& transitionData);
	void CheckTriggers();
	bool CheckConditions(const std::vector<Condition>& conditions);
	void ChangeState(const std::string& newState);
	void DrawBlended(Vector2 position, float scale);
	void DoUpdate(float deltaTime);
	void DoBlending(float deltaTime);
	void DoDrawTexture(const Animation& anim, Vector2 position, float scale, float blendFactor) const;
	int GetFirstFrame();
	int GetLastFrame();
};
