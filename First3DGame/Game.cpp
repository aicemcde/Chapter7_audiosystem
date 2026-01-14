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
#include "AudioSystem.h"
#include "AudioComponent.h"


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

	mScene = std::make_unique<Scene>(this);
	mResourceManager = std::make_unique<ResourceManager>(this);
	mRenderer = std::make_unique<Renderer>(this);

	mAudioSystem = std::make_unique<AudioSystem>(this);
	if (!mAudioSystem->Initialize())
	{
		SDL_Log("Failed to initialize audio system");
		mAudioSystem->Shutdown();
		mAudioSystem = nullptr;
		return false;
	}

	LoadData();

	mTicksCount = SDL_GetTicks();

	return true;
}

void Game::Shutdown()
{
	UnloadData();
	mAudioSystem->Shutdown();
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
		case SDL_KEYDOWN:
			if (!event.key.repeat)
			{
				HandleKeyPress(event.key.keysym.sym);
			}
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

void Game::HandleKeyPress(int key)
{
	switch (key)
	{
	case '-':
	{
		float volume = mAudioSystem->GetBusVolume("bus:/");
		volume = Math::Max(0.0f, volume - 0.1f);
		mAudioSystem->SetBusVolume("bus:/", volume);
		break;
	}
	case '=':
	{
		float volume = mAudioSystem->GetBusVolume("bus:/");
		volume = Math::Min(1.0f, volume + 0.1f);
		mAudioSystem->SetBusVolume("bus:/", volume);
		break;
	}
	case 'e':
		mAudioSystem->PlayEvent("event:/Explosion2D");
		break;
	case 'm':
		mMusicEvent.SetPaused(!mMusicEvent.GetPaused());
		break;
	case 'r':
		if (!mReverbSnap.IsValid())
		{
			mReverbSnap = mAudioSystem->PlayEvent("snapshot:/WithReverb");
		}
		else
		{
			mReverbSnap.Stop();
		}
		break;
	case '1':
		mCameraActor->SetFootstepSurface(0.0f);
		break;
	case '2':
		mCameraActor->SetFootstepSurface(0.5f);
		break;
	default:
		break;
	}
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
	mAudioSystem->SetListener(mRenderer->GetView());
	mAudioSystem->Update(deltaTime);
	//ColorfulBG(deltaTime);
}

void Game::GenerateOutput()
{
	mRenderer->Draw();
}

void Game::LoadData()
{
	//Actor
	Actor* a = mScene->CreateActor<Actor>(this);
	a->SetPosition(Vector3(200.0f, 75.0f, 0.0f));
	a->SetScale(100.0f);
	Quaternion q(Vector3::UnitY, -Math::PiOver2);
	q = Quaternion::Concatenate(q, Quaternion(Vector3::UnitZ, Math::Pi + Math::Pi / 4.0f));
	a->SetRotation(q);
	MeshComponent* mc = a->AddComponent_Pointer<MeshComponent>(a);
	mc->SetMesh(mResourceManager->GetMesh("Assets/Cube.gpmesh"));

	a = mScene->CreateActor<Actor>(this);
	a->SetPosition(Vector3(200.0f, -75.0f, 0.0f));
	a->SetScale(3.0f);
	mc = a->AddComponent_Pointer<MeshComponent>(a);
	mc->SetMesh(mResourceManager->GetMesh("Assets/Sphere.gpmesh"));

	//Floor
	const float start = -1250.0f;
	const float size = 250.0f;
	for (int i = 0; i < 10; ++i)
	{
		for (int j = 0; j < 10; ++j)
		{
			a = mScene->CreateActor<PlaneActor>(this);
			a->SetPosition(Vector3(start + i * size, start + j * size, -100.0f));
		}
	}

	q = Quaternion(Vector3::UnitX, Math::PiOver2);
	for (int i = 0; i < 10; ++i)
	{
		a = mScene->CreateActor<PlaneActor>(this);
		a->SetPosition(Vector3(start + i * size, start - size, 0.0f));
		a->SetRotation(q);

		a = mScene->CreateActor<PlaneActor>(this);
		a->SetPosition(Vector3(start + i * size, -start + size, 0.0f));
		a->SetRotation(q);
	}

	q = Quaternion::Concatenate(q, Quaternion(Vector3::UnitZ, Math::PiOver2));
	for (int i = 0; i < 10; ++i)
	{
		a = mScene->CreateActor<PlaneActor>(this);
		a->SetPosition(Vector3(start - size, start + i * size, 0.0f));
		a->SetRotation(q);

		a = mScene->CreateActor<PlaneActor>(this);
		a->SetPosition(Vector3(-start + size, start + i * size, 0.0f));
		a->SetRotation(q);
	}

	//Light
	mRenderer->SetAmbientLight(Vector3(0.2f, 0.2f, 0.2f));
	DirectionalLight& dir = mRenderer->GetDirectionalLight();
	dir.mDirection = Vector3(0.0f, -0.70f, -0.70f);
	dir.mDiffuseColor = Vector3(0.78f, 0.88f, 1.0f);
	dir.mSpecColor = Vector3(0.8f, 0.8f, 0.8f);

	//Camera
	mCameraActor = mScene->CreateActor<CameraActor>(this);

	//UI
	a = mScene->CreateActor<Actor>(this);
	a->SetPosition(Vector3(-350.0f, -350.0f, 0.0f));
	SpriteComponent* sc = a->AddComponent_Pointer<SpriteComponent>(a);
	sc->SetTexture(mResourceManager->GetTexture("Assets/HealthBar.png"));

	a = mScene->CreateActor<Actor>(this);
	a->SetPosition(Vector3(375.0f, -275.0f, 0.0f));
	a->SetScale(0.75f);
	sc = a->AddComponent_Pointer<SpriteComponent>(a);
	sc->SetTexture(mResourceManager->GetTexture("Assets/Radar.png"));

	//spheres with audio
	a = mScene->CreateActor<Actor>(this);
	a->SetPosition(Vector3(500.0f, -75.0f, 0.0f));
	a->SetScale(1.0f);
	mc = a->AddComponent_Pointer<MeshComponent>(a);
	mc->SetMesh(mResourceManager->GetMesh("Assets/Sphere.gpmesh"));
	AudioComponent* ac = a->AddComponent_Pointer<AudioComponent>(a);
	ac->PlayEvent("event:/FireLoop");

	//start music
	mMusicEvent = mAudioSystem->PlayEvent("event:/Music");
}

void Game::UnloadData()
{
	mRenderer->UnloadData();
	mScene->Unload();
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