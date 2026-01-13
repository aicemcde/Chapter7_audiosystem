#pragma once
#include <memory>
#include <vector>
#include <SDL.h>
#include <concepts>
#include "Actor.h"


class Scene
{
public:
	template<typename T, typename... Args>
	requires std::derived_from<T, class Actor>
	T* CreateActor(Args&&... args)
	{
		auto newActor = std::make_unique<T>(std::forward<Args>(args)...);
		T* ptr = newActor.get();
		if (mUpdatingActors)
		{
			mPendingActors.emplace_back(std::move(newActor));
		}
		else
		{
			mActors.emplace_back(std::move(newActor));
		}
		return ptr;
	}
	Scene(class Game* game);

	void Update(float deltaTime);
	void Unload();
	void ProcessInput(const uint8_t* keyState);

	void AddActor(std::unique_ptr<class Actor> actor);
	void RemoveActor(class Actor* actor);

	void InputActor(const uint8_t* keyState);

private:
	std::vector<std::unique_ptr<class Actor>> mActors;
	std::vector < std::unique_ptr<class Actor>> mPendingActors;
	class Game* mGame;

	bool mUpdatingActors;
};