#pragma once
#include "Math.h"
#include <SDL.h>
#include <glew.h>
#include <memory>
#include <vector>
#include "SoundEvent.h"

class Game
{
public:
	Game();
	~Game();
	bool Initialize();
	void RunLoop();
	void Shutdown();

	static Game& Get() { return *sInstance; }
	static class Scene* GetSceneInstance() { return sInstance->mScene.get(); }
	static class ResourceManager* GetResourceInstance() { return sInstance->mResourceManager.get(); }
	static class Renderer* GetRendererInstance() { return sInstance->mRenderer.get(); }
	static class AudioSystem* GetAudioSystemInstance() { return sInstance->mAudioSystem.get(); }

	class Scene* GetScene() const { return mScene.get(); }
	class ResourceManager* GetResourceManager() const { return mResourceManager.get(); }
	const Vector2& GetScreenSize() const { return mScreenSize; }
	void SetGameRunning(bool running) { mIsRunning = running; }
private:
	void ProcessInput();
	void HandleKeyPress(int key);
	void UpdateGame();
	void GenerateOutput();
	void LoadData();
	void UnloadData();
	void ColorfulBG(float deltaTime);
	

	static Game* sInstance;
	Vector2 mScreenSize;

	bool mIsRunning = false;

	std::unique_ptr<class Scene> mScene;
	std::unique_ptr<class ResourceManager> mResourceManager;
	std::unique_ptr<class Renderer> mRenderer;
	std::unique_ptr<class AudioSystem> mAudioSystem;

	class CameraActor* mCameraActor;
	SoundEvent mMusicEvent;
	SoundEvent mReverbSnap;

	Uint32 mTicksCount;

	Vector3 mRGB_bgColor;
};