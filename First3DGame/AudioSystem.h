#pragma once
#include <unordered_map>
#include <string>
#include "SoundEvent.h"
#include "Math.h"

namespace FMOD
{
	class System;
	namespace Studio
	{
		class System;
		class Bank;
		class EventDescription;
		class EventInstance;
		class Bus;
	}
}

class AudioSystem
{
public:
	AudioSystem(class Game* game);
	~AudioSystem();

	bool Initialize();
	void Shutdown();
	void Update(float deltaTime);
	void LoadBank(const std::string& name);
	void UnloadBank(const std::string& name);
	void UnloadAllBank();
	class SoundEvent PlayEvent(const std::string& name);
	void SetListener(const Matrix4& viewMatrix, float delatTime);

	float GetBusVolume(const std::string& name) const;
	bool GetBusPaused(const std::string& name) const;
	void SetBusVolume(const std::string& name, float volume);
	void SetBusPaused(const std::string& name, bool pause);
protected:
	FMOD::Studio::EventInstance* GetEventInstance(unsigned int id);
	friend class SoundEvent;
private:
	void LoadBus(FMOD::Studio::Bank* bank);
	void UnloadBus(FMOD::Studio::Bank* bank);

	static unsigned int sNextID;
	class Game* mGame;
	Vector3 mLastListenerPos;
	FMOD::Studio::System* mSystem;
	FMOD::System* mLowLevelSystem;
	std::unordered_map < std::string, FMOD::Studio::Bank* > mBanks;
	std::unordered_map<std::string, FMOD::Studio::EventDescription*> mEvents;
	std::unordered_map<unsigned int, FMOD::Studio::EventInstance*> mEventInstance;
	std::unordered_map<std::string, FMOD::Studio::Bus*> mBuses;
};