#pragma once
#include <cstdint>
#include "Math.h"
#include <vector>
#include <memory>

class Actor
{
public:

	enum State
	{
		EActive,
		EPouse,
		EDead
	};

	explicit Actor(class Game* game);
	virtual ~Actor();

	void Update(float deltaTime);
	void UpdateComponents(float deltaTime);
	virtual void UpdateActor(float deltaTime);

	void ProcessInput(const uint8_t* keyState);
	virtual void ActorInput(const uint8_t* keyState);

	void ComputeWorldTransform();

	State GetState() const { return mState; }
	Matrix4 GetWorldTransform() const { return mWorldTransform; }

	Quaternion GetRotation() const { return mRotation; }
	void SetRotation(const Quaternion& q) { mRotation = q; mRecomputeWorldTransform = true; }
	Vector3 GetPosition() const { return mPosition; }
	void SetPosition(const Vector3& pos) { mPosition = pos; mRecomputeWorldTransform = true; }
	float GetScale() const { return mScale; }
	void SetScale(float scale) { mScale = scale; mRecomputeWorldTransform = true; }
	void SetState(const State& state) { mState = state; }

	Vector3 GetForward() const { return Vector3::Transform(Vector3::UnitX, mRotation); }

	void AddComponent(std::unique_ptr<class Component> component);
	void RemoveComponent(class Component* component);
protected:
	class Game* mGame;
	State mState;
	float mScale;
	Quaternion mRotation;
	Vector3 mPosition;

	//オブジェクト空間からワールド空間への変換に用いる
	Matrix4 mWorldTransform;

	//スケールや回転、座標を変更したら、mWorldTransformをもう一度計算し直す
	bool mRecomputeWorldTransform;

	std::vector<std::unique_ptr<class Component>> mComponents;
};