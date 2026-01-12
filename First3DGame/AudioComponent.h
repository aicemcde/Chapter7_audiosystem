#include "SoundEvent.h"
#include "Component.h"
#include <string>
#include <vector>

class AudioComponent : public Component
{
public:
	AudioComponent(class Actor* owner, int updateOrder = 200);
	~AudioComponent();

	void Update(float deltaTime) override;
	void OnUpdateWorldTransform() override;

	SoundEvent PlayEvent(const std::string& name);
	void StopAllEvent();
private:
	std::vector<SoundEvent> mEvent2D;
	std::vector<SoundEvent> mEvent3D;
};