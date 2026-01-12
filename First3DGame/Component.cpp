#include "Component.h"

Component::Component(Actor* owner, int updateOrder)
	:mUpdateOrder(updateOrder)
	,mOwner(owner)
{

}

void Component::Update(float deltaTime)
{

}