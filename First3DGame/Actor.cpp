#include "Actor.h"


Actor::Actor(class Game* game)
	:mGame(game)
	,mRecomputeWorldTransform(true)
	,mScale(1.0f)
	,mPosition(Vector3::Zero)
	,mRotation()
	,mState(EActive)
{

}

void Actor::Update(float deltaTime)
{
	if (mState == EActive)
	{
		ComputeWorldTransform(deltaTime);
		UpdateComponents(deltaTime);
		UpdateActor(deltaTime);
		ComputeWorldTransform(deltaTime);
	}
}

void Actor::UpdateComponents(float deltaTime)
{
	for (const auto& comp : mComponents)
	{
		comp->Update(deltaTime);
	}
}

void Actor::UpdateActor(float deltaTime)
{

}

void Actor::ProcessInput(const uint8_t* keyState)
{
	if (mState == EActive)
	{
		for (const auto& comp : mComponents)
		{
			comp->ProcessInput(keyState);
		}
		ActorInput(keyState);
	}
}

void Actor::ActorInput(const uint8_t* keyState)
{

}

void Actor::RemoveComponent(Component* component)
{
	auto iter = std::find_if(mComponents.begin(), mComponents.end(),
		[component](const std::unique_ptr<Component>& ptr)
		{
			return ptr.get() == component;
		});
	if (iter != mComponents.end())
	{
		mComponents.erase(iter);
	}
}

void Actor::ComputeWorldTransform(float deltaTime)
{
	if (mRecomputeWorldTransform)
	{
		mRecomputeWorldTransform = false;
		mWorldTransform = Matrix4::CreateScale(mScale);
		mWorldTransform *= Matrix4::CreateFromQuaternion(mRotation);
		mWorldTransform *= Matrix4::CreateTranslation(mPosition);

		for (const auto& comp : mComponents)
		{
			comp->OnUpdateWorldTransform(deltaTime);
		}
	}
}