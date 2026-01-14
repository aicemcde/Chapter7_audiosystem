#pragma once
#include <cstdint>
#include "Math.h"
#include <vector>
#include <memory>
#include <concepts>
#include <algorithm>
#include "Component.h"

class Actor
{
public:

	enum State
	{
		EActive,
		EPouse,
		EDead
	};

	template<typename T, typename... Args>
	requires std::derived_from<T, class Component>
	void AddComponent(Args&&... args)
	{
		auto newComponent = std::make_unique<T>(std::forward<Args>(args)...);
		int myOrder = newComponent->GetUpdateOrder();
		auto iter = std::ranges::lower_bound(mComponents, myOrder, {}, &Component::GetUpdateOrder);
		mComponents.insert(iter, std::move(newComponent));
	}

	template<typename T,typename... Args>
	requires std::derived_from<T, class Component>
	T* AddComponent_Pointer(Args&&... args)
	{
		auto newComponent = std::make_unique<T>(std::forward<Args>(args)...);
		T* ptr = newComponent.get();
		int myOrder = newComponent->GetUpdateOrder();
		auto iter = std::ranges::lower_bound(mComponents, myOrder, {}, &Component::GetUpdateOrder);
		mComponents.insert(iter, std::move(newComponent));
		return ptr;
	}
	
	explicit Actor(class Game* game);
	virtual ~Actor()=default;

	void Update(float deltaTime);
	void UpdateComponents(float deltaTime);
	virtual void UpdateActor(float deltaTime);

	void ProcessInput(const uint8_t* keyState);
	virtual void ActorInput(const uint8_t* keyState);

	void ComputeWorldTransform(float deltaTime);

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

	void RemoveComponent(class Component* component);
protected:
	class Game* mGame;
	State mState;

	//ワールド空間での座標
	Matrix4 mWorldTransform;

	//スケールや回転、座標を変更したら、mWorldTransformをもう一度計算し直す
	bool mRecomputeWorldTransform;

	std::vector<std::unique_ptr<class Component>> mComponents;

private:
	//↓オブジェクト空間
	float mScale;
	Quaternion mRotation;
	Vector3 mPosition;
};