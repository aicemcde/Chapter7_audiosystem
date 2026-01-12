#include "AudioComponent.h"
#include "Game.h"
#include "AudioSystem.h"
#include "Actor.h"
#include <algorithm>

AudioComponent::AudioComponent(Actor* actor, int updateOrder)
	:Component(actor, updateOrder)
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

void AudioComponent::OnUpdateWorldTransform()
{
	Matrix4 world = mOwner->GetWorldTransform();
	for (auto& event : mEvent3D)
	{
		if (event.IsValid())
		{
			event.Set3DAttributes(world);
		}
	}
}

SoundEvent AudioComponent::PlayEvent(const std::string& name)
{
	SoundEvent e = Game::GetAudioSystemInstance()->PlayEvent(name);
	if (e.Is3D())
	{
		mEvent3D.emplace_back(e);
		e.Set3DAttributes(mOwner->GetWorldTransform());
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