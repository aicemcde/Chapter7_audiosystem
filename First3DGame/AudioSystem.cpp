#include "AudioSystem.h"
#include <fmod_studio.hpp>
#include "Game.h"
#include <fmod_errors.h>

const int MAX_PATH_LENGTH = 512;

unsigned int AudioSystem::sNextID = 0;

AudioSystem::AudioSystem(Game* game)
	:mGame(game)
{
	Initialize();
}

AudioSystem::~AudioSystem()
{

}

bool AudioSystem::Initialize()
{
	FMOD::Debug_Initialize(
		FMOD_DEBUG_LEVEL_ERROR,
		FMOD_DEBUG_MODE_TTY
	);

	FMOD_RESULT result;
	result = FMOD::Studio::System::create(&mSystem);
	if (result != FMOD_OK)
	{
		SDL_Log("Failed to create FMOD system : %s", FMOD_ErrorString(result));
		return false;
	}

	result = mSystem->initialize(
		512,
		FMOD_STUDIO_INIT_NORMAL,
		FMOD_INIT_NORMAL,
		nullptr
	);

	mSystem->getLowLevelSystem(&mLowLevelSystem);

	LoadBank("Assets/Master Bank.strings.bank");
	LoadBank("Assets/Master Bank.bank");

	return true;
}

void AudioSystem::Shutdown()
{
	mSystem->release();
}

void AudioSystem::Update(float deltaTime)
{
	std::vector<unsigned int> done;
	for (auto& iter : mEventInstance)
	{
		FMOD::Studio::EventInstance* e = iter.second;
		FMOD_STUDIO_PLAYBACK_STATE state;
		e->getPlaybackState(&state);
		if (state == FMOD_STUDIO_PLAYBACK_STOPPED)
		{
			e->release();
			done.emplace_back(iter.first);
		}
	}
	for (auto id : done)
	{
		mEventInstance.erase(id);
	}
	mSystem->update();
}

void AudioSystem::LoadBank(const std::string& name)
{
	if (mBanks.find(name) != mBanks.end())
	{
		return;
	}

	FMOD::Studio::Bank* bank = nullptr;
	FMOD_RESULT result = mSystem->loadBankFile(
		name.c_str(),
		FMOD_STUDIO_LOAD_BANK_NORMAL,
		&bank
	);

	if (result == FMOD_OK)
	{
		mBanks.emplace(name, bank);
		bank->loadSampleData();
		int numEvents = 0;
		bank->getEventCount(&numEvents);
		if (numEvents > 0)
		{
			std::vector<FMOD::Studio::EventDescription*> events(numEvents);
			bank->getEventList(events.data(), numEvents, &numEvents);
			char eventName[MAX_PATH_LENGTH];
			for (int i = 0; i < numEvents; ++i)
			{
				FMOD::Studio::EventDescription* e = events[i];
				e->getPath(eventName, MAX_PATH_LENGTH, nullptr);
				mEvents.emplace(eventName, e);
			}
		}
	}
}

void AudioSystem::UnloadBank(const std::string& name)
{
	auto iter = mBanks.find(name);
	if (iter == mBanks.end())
	{
		return;
	}

	FMOD::Studio::Bank* bank = iter->second;
	int numEvents = 0;
	bank->getEventCount(&numEvents);
	if (numEvents > 0)
	{
		std::vector<FMOD::Studio::EventDescription*> events(numEvents);
		bank->getEventList(events.data(), numEvents, &numEvents);
		char eventName[MAX_PATH_LENGTH];
		for (int i = 0; i < numEvents; ++i)
		{
			FMOD::Studio::EventDescription* e = events[i];
			e->getPath(eventName, MAX_PATH_LENGTH, nullptr);
			auto event_iter = mEvents.find(eventName);
			if (event_iter != mEvents.end())
			{
				mEvents.erase(event_iter);
			}
		}
	}
	bank->unloadSampleData();
	bank->unload();
	mBanks.erase(iter);
}

void AudioSystem::UnloadAllBank()
{
	for (auto& iter : mBanks)
	{
		iter.second->unloadSampleData();
		iter.second->unload();
	}
	mBanks.clear();
	mEvents.clear();
}

SoundEvent AudioSystem::PlayEvent(const std::string& name)
{
	unsigned int retID = 0;
	auto iter = mEvents.find(name);
	if (iter != mEvents.end())
	{
		FMOD::Studio::EventInstance* event = nullptr;
		iter->second->createInstance(&event);
		if (event)
		{
			event->start();
			++sNextID;
			retID = sNextID;
			mEventInstance.emplace(retID, event);
		}
	}
	return SoundEvent();
}

FMOD::Studio::EventInstance* AudioSystem::GetEventInstance(unsigned int id)
{
	FMOD::Studio::EventInstance* event = nullptr;
	auto iter = mEventInstance.find(id);
	if (iter != mEventInstance.end())
	{
		event = iter->second;
	}
	return event;
}