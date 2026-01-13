#include "PlaneActor.h"
#include "MeshComponent.h"
#include "Game.h"
#include "ResourceManager.h"
#include "Mesh.h"

PlaneActor::PlaneActor(class Game* game)
	:Actor(game)
{
	SetScale(10.0f);
	MeshComponent* mc = AddComponent_Pointer<MeshComponent>(this);
	mc->SetMesh(mGame->GetResourceInstance()->GetMesh("Assets/Plane.gpmesh"));
}