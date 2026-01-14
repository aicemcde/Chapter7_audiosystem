#include "AudioComponent.h"
#include "Game.h"
#include "AudioSystem.h"
#include "Actor.h"
#include <algorithm>

namespace
{
	Vector3 CalVelocity(const Vector3& current, const Vector3& last, float deltaTime)
	{
		Vector3 result;
		result.x = (current.x - last.x) / deltaTime;
		result.y = (current.y - last.y) / deltaTime;
		result.z = (current.z - last.z) / deltaTime;
		return result;
	}
}


AudioComponent::AudioComponent(Actor* actor, int updateOrder)
	:Component(actor, updateOrder)
	, mLastPos(Vector3::Zero)
{

}

AudioComponent::~AudioComponent()
{
	StopAllEvent();
}

void AudioComponent::Update(float deltaTime)
{
	Component::Update(deltaTime);

	std::erase_if(mEvent2D, [](const SoundEvent& e)
		{
			return !e.IsValid();
		}
	);

	std::erase_if(mEvent3D, [](const SoundEvent& e)
		{
			return !e.IsValid();
		}
	);

}

void AudioComponent::OnUpdateWorldTransform(float deltaTime)
{
	Matrix4 world = mOwner->GetWorldTransform();
	Vector3 currentPos = world.GetTranslation();
	Vector3 velocity = Vector3::Zero;
	if (deltaTime > 0)
	{
		velocity = CalVelocity(currentPos, mLastPos, deltaTime);
	}

	for (auto& event : mEvent3D)
	{
		if (event.IsValid())
		{
			event.Set3DAttributes(world, velocity);
		}
	}

	mLastPos = currentPos;
}

SoundEvent AudioComponent::PlayEvent(const std::string& name)
{
	SoundEvent e = Game::GetAudioSystemInstance()->PlayEvent(name);
	if (e.Is3D())
	{
		mEvent3D.emplace_back(e);
		e.Set3DAttributes(mOwner->GetWorldTransform(), Vector3(0, 0, 0));
	}
	else
	{
		mEvent2D.emplace_back(e);
	}
	return e;
}

void AudioComponent::StopAllEvent()
{
	for (auto& event : mEvent2D)
	{
		event.Stop();
	}
	for (auto& event : mEvent3D)
	{
		event.Stop();
	}
	mEvent2D.clear();
	mEvent3D.clear();
}