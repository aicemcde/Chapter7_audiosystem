#include "Game.h"
#include "VertexArray.h"
#include "Shader.h"
#include <cstdint>
#include "Scene.h"
#include "Actor.h"
#include "ResourceManager.h"
#include <SDL_ttf.h>
#include "Renderer.h"
#include "MeshComponent.h"
#include "CameraActor.h"
#include "Mesh.h"
#include "PlaneActor.h"
#include "SpriteComponent.h"


Game* Game::sInstance = nullptr;

Game::Game()
	:mIsRunning(true)
	, mTicksCount(0)
	, mScreenSize(1024.0f, 768.0f)
	, mCameraActor(nullptr)
{
	if (sInstance == nullptr)
	{
		sInstance = this;
	}
}

Game::~Game()
{

}

bool Game::Initialize()
{
	int sdlResult = SDL_Init(SDL_INIT_VIDEO);
	if (sdlResult != 0)
	{
		SDL_Log("SDL could not initialize! SDL_Error : %s", SDL_GetError());
		return false;
	}

	mScene = std::make_unique<Scene>();
	mResourceManager = std::make_unique<ResourceManager>(this);
	mRenderer = std::make_unique<Renderer>(this);
	if (!mRenderer->Initialize())
	{
		SDL_Log("Renderer could not initialize!");
		return false;
	}

	LoadData();

	return true;
}

void Game::Shutdown()
{
	UnloadData();
	SDL_Quit();
}

void Game::RunLoop()
{
	GLenum err;
	while (mIsRunning)
	{
		ProcessInput();
		UpdateGame();
		GenerateOutput();
		while ((err = glGetError()) != GL_NO_ERROR)
		{
			SDL_Log("GL_Error : %d", err);
		}
	}
}

void Game::ProcessInput()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		default:
			break;

		case SDL_QUIT:
			mIsRunning = false;
			break;
		}
	}

	const uint8_t* keyState = SDL_GetKeyboardState(NULL);
	if (keyState[SDL_SCANCODE_ESCAPE])
	{
		mIsRunning = false;
	}

	mScene->ProcessInput(keyState);

}

void Game::UpdateGame()
{
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16));
	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
	mTicksCount = SDL_GetTicks();
	if (deltaTime > 0.05f)
	{
		deltaTime = 0.05f;
	}
	mScene->Update(deltaTime);
	//ColorfulBG(deltaTime);
}

void Game::GenerateOutput()
{
	mRenderer->Draw();
}

void Game::LoadData()
{
	std::unique_ptr<Actor> a = std::make_unique<Actor>(this);
	a->SetPosition(Vector3(200.0f, 75.0f, 0.0f));
	a->SetScale(100.0f);
	Quaternion q(Vector3::UnitY, -Math::PiOver2);
	q = Quaternion::Concatenate(q, Quaternion(Vector3::UnitZ, Math::Pi + Math::Pi / 4.0f));
	a->SetRotation(q);
	std::unique_ptr<MeshComponent> mc = std::make_unique<MeshComponent>(a.get());
	mc->SetMesh(mResourceManager->GetMesh("Assets/Cube.gpmesh"));
	a->AddComponent(std::move(mc));
	mScene->AddActor(std::move(a));

	a = std::make_unique<Actor>(this);
	a->SetPosition(Vector3(200.0f, -75.0f, 0.0f));
	a->SetScale(3.0f);
	mc = std::make_unique<MeshComponent>(a.get());
	mc->SetMesh(mResourceManager->GetMesh("Assets/Sphere.gpmesh"));
	a->AddComponent(std::move(mc));
	mScene->AddActor(std::move(a));

	const float start = -1250.0f;
	const float size = 250.0f;
	for (int i = 0; i < 10; ++i)
	{
		for (int j = 0; j < 10; ++j)
		{
			a = std::make_unique<PlaneActor>(this);
			a->SetPosition(Vector3(start + i * size, start + j * size, -100.0f));
			mScene->AddActor(std::move(a));
		}
	}

	q = Quaternion(Vector3::UnitX, Math::PiOver2);
	for (int i = 0; i < 10; ++i)
	{
		a = std::make_unique<PlaneActor>(this);
		a->SetPosition(Vector3(start + i * size, start - size, 0.0f));
		a->SetRotation(q);
		mScene->AddActor(std::move(a));

		a = std::make_unique<PlaneActor>(this);
		a->SetPosition(Vector3(start + i * size, -start + size, 0.0f));
		a->SetRotation(q);
		mScene->AddActor(std::move(a));
	}

	q = Quaternion::Concatenate(q, Quaternion(Vector3::UnitZ, Math::PiOver2));
	for (int i = 0; i < 10; ++i)
	{
		a = std::make_unique<PlaneActor>(this);
		a->SetPosition(Vector3(start - size, start + i * size, 0.0f));
		a->SetRotation(q);
		mScene->AddActor(std::move(a));

		a = std::make_unique<PlaneActor>(this);
		a->SetPosition(Vector3(-start + size, start + i * size, 0.0f));
		a->SetRotation(q);
		mScene->AddActor(std::move(a));
	}

	mRenderer->SetAmbientLight(Vector3(0.2f, 0.2f, 0.2f));
	DirectionalLight& dir = mRenderer->GetDirectionalLight();
	dir.mDirection = Vector3(0.0f, -0.70f, -0.70f);
	dir.mDiffuseColor = Vector3(0.78f, 0.88f, 1.0f);
	dir.mSpecColor = Vector3(0.8f, 0.8f, 0.8f);

	std::unique_ptr<CameraActor> ca = std::make_unique<CameraActor>(this);
	mCameraActor = ca.get();
	mScene->AddActor(std::move(ca));

	a = std::make_unique<Actor>(this);
	a->SetPosition(Vector3(-350.0f, -350.0f, 0.0f));
	std::unique_ptr<SpriteComponent> sc = std::make_unique<SpriteComponent>(a.get());
	sc->SetTexture(mResourceManager->GetTexture("Assets/HealthBar.png"));
	a->AddComponent(std::move(sc));
	mScene->AddActor(std::move(a));

	a = std::make_unique<Actor>(this);
	a->SetPosition(Vector3(375.0f, -275.0f, 0.0f));
	a->SetScale(0.75f);
	sc = std::make_unique<SpriteComponent>(a.get());
	sc->SetTexture(mResourceManager->GetTexture("Assets/Radar.png"));
	a->AddComponent(std::move(sc));
	mScene->AddActor(std::move(a));
}

void Game::UnloadData()
{
	mRenderer->UnloadData();
}

void Game::ColorfulBG(float deltaTime)
{
	if (mRGB_bgColor.z < 1.0f)
	{
		mRGB_bgColor.z += deltaTime / 5.0f;
		if (mRGB_bgColor.z > 1.0f)
		{
			mRGB_bgColor.z = 1.0f;
		}
	}
	glClearColor(mRGB_bgColor.x, mRGB_bgColor.y, mRGB_bgColor.z, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}