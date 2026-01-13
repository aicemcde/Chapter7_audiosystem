#pragma once
#include <cstdint>

class Component
{
public:
	Component(class Actor* owner, int updateOrder = 100);
	virtual ~Component()=default;

	virtual void Update(float deltaTime);
	virtual void ProcessInput(const uint8_t* keyState) {};
	virtual void OnUpdateWorldTransform() {};
	int GetUpdateOrder() const { return mUpdateOrder; }
	class Actor* GetOwner() const { return mOwner; }
protected:
	int mUpdateOrder;
	class Actor* mOwner;
};