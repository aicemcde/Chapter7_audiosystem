#include "Actor.h"
#include "Component.h"
#include <algorithm>

Actor::Actor(class Game* game)
	:mGame(game)
	,mRecomputeWorldTransform(true)
	,mScale(1.0f)
	,mPosition(Vector3::Zero)
	,mRotation()
	,mState(EActive)
{

}

Actor::~Actor()
{

}

void Actor::Update(float deltaTime)
{
	if (mState == EActive)
	{
		ComputeWorldTransform();
		UpdateComponents(deltaTime);
		UpdateActor(deltaTime);
		ComputeWorldTransform();
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

void Actor::AddComponent(std::unique_ptr<Component> component)
{
	int myOrder = component->GetUpdateOrder();
	auto iter = std::ranges::lower_bound(mComponents, myOrder, {}, &Component::GetUpdateOrder);
	mComponents.insert(iter, std::move(component));
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

void Actor::ComputeWorldTransform()
{
	if (mRecomputeWorldTransform)
	{
		mRecomputeWorldTransform = false;
		mWorldTransform = Matrix4::CreateScale(mScale);
		mWorldTransform *= Matrix4::CreateFromQuaternion(mRotation);
		mWorldTransform *= Matrix4::CreateTranslation(mPosition);
	}
}