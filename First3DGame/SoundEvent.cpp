#include "SoundEvent.h"
#include "AudioSystem.h"
#include <fmod_studio.hpp>

SoundEvent::SoundEvent(AudioSystem* system, unsigned int id)
	:mSystem(system)
	,mID(id)
{

}

SoundEvent::SoundEvent()
	:mSystem(nullptr)
	, mID(0)
{

}

bool SoundEvent::IsValid() const
{
	return (mSystem && mSystem->GetEventInstance(mID) != nullptr);
}

void SoundEvent::Restart()
{
	auto event = mSystem ? mSystem->GetEventInstance(mID) : nullptr;
	if (event)
	{
		event->start();
	}
}

void SoundEvent::Stop(bool allowFadeOut)
{
	auto event = mSystem ? mSystem->GetEventInstance(mID) : nullptr;
	if (event)
	{
		FMOD_STUDIO_STOP_MODE mode = allowFadeOut ?
			FMOD_STUDIO_STOP_ALLOWFADEOUT :
			FMOD_STUDIO_STOP_IMMEDIATE;
		event->stop(mode);
	}
}

void SoundEvent::SetPaused(bool pause)
{
	auto event = mSystem ? mSystem->GetEventInstance(mID) : nullptr;
	if (event)
	{
		event->setPaused(pause);
	}
}

void SoundEvent::SetVolume(float value)
{
	auto event = mSystem ? mSystem->GetEventInstance(mID) : nullptr;
	if (event)
	{
		event->setVolume(value);
	}
}

void SoundEvent::SetPitch(float value)
{
	auto event = mSystem ? mSystem->GetEventInstance(mID) : nullptr;
	if (event)
	{
		event->setPitch(value);
	}
}

void SoundEvent::SetParameter(const std::string& name, float value)
{
	auto event = mSystem ? mSystem->GetEventInstance(mID) : nullptr;
	if (event)
	{
		event->setParameterValue(name.c_str(), value);
	}
}

bool SoundEvent::GetPaused() const
{
	auto event = mSystem ? mSystem->GetEventInstance(mID) : nullptr;
	bool pause = false;
	if (event)
	{
		event->getPaused(&pause);
	}
	return pause;
}

float SoundEvent::GetVolume() const
{
	auto event = mSystem ? mSystem->GetEventInstance(mID) : nullptr;
	float volume = 0.0f;
	if (event)
	{
		event->getVolume(&volume);
	}
	return volume;
}

float SoundEvent::GetPitch() const
{
	auto event = mSystem ? mSystem->GetEventInstance(mID) : nullptr;
	float value = 0.0f;
	if (event)
	{
		event->getPitch(&value);
	}
	return value;
}

float SoundEvent::GetParameter(const std::string& name) const
{
	auto event = mSystem ? mSystem->GetEventInstance(mID) : nullptr;
	float value = 0.0f;
	if (event)
	{
		event->getParameterValue(name.c_str(), &value);
	}
	return value;
}

bool SoundEvent::Is3D() const
{
	bool retVal = false;
	auto event = mSystem ? mSystem->GetEventInstance(mID) : nullptr;
	if (event)
	{
		FMOD::Studio::EventDescription* ed = nullptr;
		event->getDescription(&ed);
		if (ed)
		{
			ed->is3D(&retVal);
		}
	}
	return retVal;
}

namespace
{
	FMOD_VECTOR VecToFMOD(const Vector3& in)
	{
		FMOD_VECTOR v;
		v.x = in.y;
		v.y = in.z;
		v.z = in.x;
		return v;
	}
}

void SoundEvent::Set3DAttributes(const Matrix4& worldTrans)
{
	auto event = mSystem ? mSystem->GetEventInstance(mID) : nullptr;
	if (event)
	{
		FMOD_3D_ATTRIBUTES attr;
		attr.position = VecToFMOD(worldTrans.GetTranslation());
		attr.forward = VecToFMOD(worldTrans.GetXAxis());
		attr.up = VecToFMOD(worldTrans.GetZAxis());
		attr.velocity = { 0.0f, 0.0f, 0.0f };
		event->set3DAttributes(&attr);
	}
}