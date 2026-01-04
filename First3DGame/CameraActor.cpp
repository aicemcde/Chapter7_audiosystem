#include "CameraActor.h"
#include "MoveComponent.h"
#include "Game.h"
#include "Renderer.h"

CameraActor::CameraActor(Game* game)
	:Actor(game)
{
	std::unique_ptr<MoveComponent> mc = std::make_unique<MoveComponent>(this);
	mMoveComp = mc.get();
	AddComponent(std::move(mc));
}

void CameraActor::UpdateActor(float deltaTime)
{
	Actor::UpdateActor(deltaTime);
	Vector3 cameraPos = GetPosition();
	Vector3 target = cameraPos + GetForward() * 100.0f;
	Vector3 up = Vector3::UnitZ;

	Matrix4 view = Matrix4::CreateLookAt(cameraPos, target, up);
	Game::GetRendererInstance()->SetViewMatrix(view);
}

void CameraActor::ActorInput(const uint8_t* keys)
{
	float forwardSpeed = 0.0f;
	float angularSpeed = 0.0f;
	if (keys[SDL_SCANCODE_W])
	{
		forwardSpeed += 300.0f;
	}
	if (keys[SDL_SCANCODE_S])
	{
		forwardSpeed -= 300.0f;
	}
	if (keys[SDL_SCANCODE_A])
	{
		angularSpeed -= Math::TwoPi;
	}
	if (keys[SDL_SCANCODE_D])
	{
		angularSpeed += Math::TwoPi;
	}

	mMoveComp->SetForwardSpeed(forwardSpeed);
	mMoveComp->SetAngularSpeed(angularSpeed);
}