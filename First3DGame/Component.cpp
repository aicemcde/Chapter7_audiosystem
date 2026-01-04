#include "Component.h"

Component::Component(Actor* owner, int updateOrder)
	:mUpdateOrder(updateOrder)
	,mOwner(owner)
{

}

Component::~Component()
{

}

void Component::Update(float deltaTime)
{

}