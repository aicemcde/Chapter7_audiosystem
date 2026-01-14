#include "AudioSystem.h"
#include <fmod_studio.hpp>
#include "Game.h"
#include <fmod_errors.h>

const int MAX_PATH_LENGTH = 512;
const float DIRECT_OCCLUSION = 1.0;
const float REVERB_OCCLUSION = 1.0;

unsigned int AudioSystem::sNextID = 0;

namespace
{
	static FMOD_VECTOR VecToFMOD(const Vector3& in)
	{
		FMOD_VECTOR v;
		v.x = in.y;
		v.y = in.z;
		v.z = in.x;
		return v;
	}
}

AudioSystem::AudioSystem(Game* game)
	:mGame(game)
{
	
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
		FMOD_INIT_CHANNEL_LOWPASS,
		nullptr
	);

	mSystem->getLowLevelSystem(&mLowLevelSystem);

	float dopplerScale = 1.0f;
	float distanceFactor = 50.0f;
	float rolloffScale = 1.0f;
	mLowLevelSystem->set3DSettings(
		dopplerScale,
		distanceFactor,
		rolloffScale
	);

	LoadBank("Assets/Master Bank.strings.bank");
	LoadBank("Assets/Master Bank.bank");

	return true;
}

void AudioSystem::Shutdown()
{
	for (auto& iter : mEventInstance)
	{
		iter.second->stop(FMOD_STUDIO_STOP_IMMEDIATE);
		iter.second->release();
	}
	mEventInstance.clear();

	UnloadAllBank();

	if (mSystem)
	{
		mSystem->release();
	}
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

		LoadBus(bank);
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
	UnloadBus(bank);
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

		mSystem->flushCommands();
		FMOD::ChannelGroup* cg = nullptr;
		event->getChannelGroup(&cg);
		cg->set3DOcclusion(DIRECT_OCCLUSION, REVERB_OCCLUSION);
	}

	if (retID > 0)
	{
		return SoundEvent(this, retID);
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

void AudioSystem::SetListener(const Matrix4& viewMatrix)
{
	Matrix4 inView = viewMatrix;
	inView.Invert();
	FMOD_3D_ATTRIBUTES listener;
	listener.position = VecToFMOD(inView.GetTranslation());
	listener.forward = VecToFMOD(inView.GetZAxis());
	listener.up = VecToFMOD(inView.GetYAxis());
	listener.velocity = { 0.0f, 0.0f, 0.0f };
	mSystem->setListenerAttributes(0, &listener);
}

void AudioSystem::LoadBus(FMOD::Studio::Bank* bank)
{
	int numBuses = 0;
	bank->getBusCount(&numBuses);
	if (numBuses > 0)
	{
		std::vector<FMOD::Studio::Bus*> buses(numBuses);
		bank->getBusList(buses.data(), numBuses, &numBuses);
		char busName[512];
		for (int i = 0; i < numBuses; ++i)
		{
			FMOD::Studio::Bus* bus = buses[i];
			bus->getPath(busName, 512, nullptr);
			mBuses.emplace(busName, bus);
		}
	}
}

void AudioSystem::UnloadBus(FMOD::Studio::Bank* bank)
{
	int numBuses = 0;
	bank->getBusCount(&numBuses);
	if (numBuses > 0)
	{
		std::vector<FMOD::Studio::Bus*> buses(numBuses);
		bank->getBusList(buses.data(), numBuses, &numBuses);
		char busName[512];
		for (int i = 0; i < numBuses; ++i)
		{
			FMOD::Studio::Bus* bus = buses[i];
			bus->getPath(busName, 512, nullptr);
			auto bus_iter = mBuses.find(busName);
			if (bus_iter != mBuses.end())
			{
				mBuses.erase(bus_iter);
			}
		}
	}
}

float AudioSystem::GetBusVolume(const std::string& name) const
{
	auto iter = mBuses.find(name);
	FMOD::Studio::Bus* bus = nullptr;
	float volume = 0.0f;
	if (iter != mBuses.end())
	{
		bus = iter->second;
		bus->getVolume(&volume);
	}
	return volume;
}

bool AudioSystem::GetBusPaused(const std::string& name) const
{
	FMOD::Studio::Bus* bus = nullptr;
	auto iter = mBuses.find(name);
	bool pause = false;
	if (iter != mBuses.end())
	{
		bus = iter->second;
		bus->getPaused(&pause);
	}
	return pause;
}

void AudioSystem::SetBusVolume(const std::string& name, float volume)
{
	FMOD::Studio::Bus* bus = nullptr;
	auto iter = mBuses.find(name);
	if (iter != mBuses.end())
	{
		bus = iter->second;
		bus->setVolume(volume);
	}
}

void AudioSystem::SetBusPaused(const std::string& name, bool pause)
{
	FMOD::Studio::Bus* bus = nullptr;
	auto iter = mBuses.find(name);
	if (iter != mBuses.end())
	{
		bus = iter->second;
		bus->setPaused(pause);
	}
}