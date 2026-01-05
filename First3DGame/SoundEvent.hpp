#include <string>

class SoundEvent
{
public:
	SoundEvent();
	bool IsValid();
	void Restart();
	void Stop(bool allowFadeOut = true);
	
	void SetPaused(bool pause);
	void SetVolume(float value);
	void SetPitch(float value);
	void SetParameter(const std::string& name, float value);
protected:
	friend class AudioSystem;
	SoundEvent(class AudioSystem* system, unsigned int id);
private:
	class AudioSystem* mSystem;
	unsigned int mID;
};