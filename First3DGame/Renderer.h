#pragma once
#include <string>
#include <vector>
#include <memory>
#include "Math.h"
#include <SDL.h>

struct DirectionalLight
{
	Vector3 mDirection;
	Vector3 mDiffuseColor;
	Vector3 mSpecColor;
};

class Renderer
{
public:
	Renderer(class Game* game);
	~Renderer();

	bool Initialize();
	void Shutdown();
	void UnloadData();
	void Draw();

	void AddSprite(class SpriteComponent* sprite);
	void RemoveSprite(class SpriteComponent* sprite);

	void AddMeshComp(class MeshComponent* meshcomp);
	void RemoveMeshComp(class MeshComponent* mc);

	void SetLightUniforms(class Shader* shader);
	void SetViewMatrix(const Matrix4& view) noexcept { mView = view; }
	void SetAmbientLight(const Vector3& ambient) noexcept { mAmbientLight = ambient; }
	DirectionalLight& GetDirectionalLight() noexcept { return mDirLight; }
private:
	bool LoadShaders();
	void CreateSpriteVerts();

	SDL_Window* mWindow = nullptr;
	SDL_GLContext mContext;

	std::vector<class SpriteComponent*> mSprites;
	std::vector<class MeshComponent*> mMeshComps;

	std::unique_ptr<class VertexArray> mSpriteVerts;
	std::unique_ptr<class Shader> mSpriteShader;
	std::unique_ptr<class Shader> mMeshShader;
	
	Matrix4 mView;
	Matrix4 mProjection;
	Vector3 mAmbientLight;
	DirectionalLight mDirLight;

	class Game* mGame;
};