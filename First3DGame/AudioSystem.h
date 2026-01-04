#pragma once
#include <unordered_map>
#include <string>

namespace FMOD
{
	class System;
	namespace Studio
	{
		class System;
		class Bank;
		class EventDescription;
		class EventInstance;
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
protected:
	FMOD::Studio::EventInstance* GetEventInstance(unsigned int id);
private:
	static unsigned int sNextID;
	class Game* mGame;
	FMOD::Studio::System* mSystem;
	FMOD::System* mLowLevelSystem;
	std::unordered_map < std::string, FMOD::Studio::Bank* > mBanks;
	std::unordered_map<std::string, FMOD::Studio::EventDescription*> mEvents;
	std::unordered_map<unsigned int, FMOD::Studio::EventInstance*> mEventInstance;
};