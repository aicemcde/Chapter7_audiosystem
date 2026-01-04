#pragma once
#include "Component.h"
#include <iostream>

class MoveComponent : public Component
{
public:
	MoveComponent(class Actor* owner, int updateOrder = 10);

	void Update(float deltaTime) override;

	float GetAngularSpeed() const noexcept { return mAngularSpeed; }
	float GetForwardSpeed() const noexcept { return mForwardSpeed; }
	void SetAngularSpeed(float speed) noexcept { mAngularSpeed = speed; }
	void SetForwardSpeed(float speed) noexcept { mForwardSpeed = speed; }
private:
	float mAngularSpeed;
	float mForwardSpeed;
};