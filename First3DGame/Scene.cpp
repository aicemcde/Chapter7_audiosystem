#include "Scene.h"
#include "Actor.h"
#include <algorithm>
#include "SpriteComponent.h"

Scene::Scene()
	:mUpdatingActors(false)
{

}

void Scene::ProcessInput(const uint8_t* keyState)
{
	mUpdatingActors = true;
	for (auto& actor : mActors)
	{
		actor->ProcessInput(keyState);
	}
	mUpdatingActors = false;
}

void Scene::Update(float deltaTime)
{
	mUpdatingActors = true;
	for (auto& actor : mActors)
	{
		actor->Update(deltaTime);
	}
	mUpdatingActors = false;

	for (auto& pending : mPendingActors)
	{
		mActors.emplace_back(std::move(pending));
	}
	mPendingActors.clear();

	auto iter = std::remove_if(mActors.begin(), mActors.end(),
		[](const std::unique_ptr<Actor>& actor)
		{
			return actor->GetState() == Actor::EDead;
		});
	mActors.erase(iter, mActors.end());
}

void Scene::Unload()
{
	mActors.clear();
}

void Scene::AddActor(std::unique_ptr<Actor> actor)
{
	if (mUpdatingActors)
	{
		mPendingActors.emplace_back(std::move(actor));
	}
	else
	{
		mActors.emplace_back(std::move(actor));
	}
}

void Scene::RemoveActor(Actor* actor)
{
	auto iter = std::find_if(mPendingActors.begin(), mPendingActors.end(),
		[actor](const std::unique_ptr<Actor>& ptr)
		{
			return ptr.get() == actor;
		});
	if (iter != mPendingActors.end())
	{
		std::iter_swap(iter, mPendingActors.end() - 1);
		mPendingActors.pop_back();
		return;
	}

	if (!mUpdatingActors)
	{
		iter = std::find_if(mActors.begin(), mActors.end(),
			[actor](const std::unique_ptr<Actor>& ptr)
			{
				return ptr.get() == actor;
			});
		if (iter != mActors.end())
		{
			std::iter_swap(iter, mActors.end() - 1);
			mActors.pop_back();
		}
	}
}

void Scene::InputActor(const uint8_t* keyState)
{
	mUpdatingActors = true;
	for (auto& actor : mActors)
	{
		actor->ProcessInput(keyState);
	}
	mUpdatingActors = false;
}