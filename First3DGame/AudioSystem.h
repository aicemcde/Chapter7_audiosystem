#pragma once

namespace FMOD
{
	class System;
	namespace Studio
	{
		class System;
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
private:
	class Game* mGame;
	FMOD::Studio::System* mSystem;
	FMOD::System* mLowLevelSystem;
};