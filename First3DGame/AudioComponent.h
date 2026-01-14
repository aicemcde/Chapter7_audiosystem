#include "SoundEvent.h"
#include "Component.h"
#include <string>
#include <vector>

class AudioComponent : public Component
{
public:
	AudioComponent(class Actor* owner, int updateOrder = 200, const Vector3& pos = Vector3::Zero);
	~AudioComponent();

	void Update(float deltaTime) override;
	void OnUpdateWorldTransform(float deltaTime) override;

	SoundEvent PlayEvent(const std::string& name);
	void StopAllEvent();
private:
	Vector3 mLastPos;
	Vector3 mLastVelocity;
	std::vector<SoundEvent> mEvent2D;
	std::vector<SoundEvent> mEvent3D;
};