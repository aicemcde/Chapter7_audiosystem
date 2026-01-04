#include "PlaneActor.h"
#include "MeshComponent.h"
#include "Game.h"
#include "ResourceManager.h"
#include "Mesh.h"

PlaneActor::PlaneActor(class Game* game)
	:Actor(game)
{
	SetScale(10.0f);
	std::unique_ptr<MeshComponent> mc = std::make_unique<MeshComponent>(this);
	mc->SetMesh(mGame->GetResourceInstance()->GetMesh("Assets/Plane.gpmesh"));
	AddComponent(std::move(mc));
}