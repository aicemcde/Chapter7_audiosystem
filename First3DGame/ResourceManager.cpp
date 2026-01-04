#include "ResourceManager.h"
#include "Texture.h"
#include "Mesh.h"
#include "Game.h"
#include "Renderer.h"

ResourceManager::ResourceManager(Game* game)
	:mGame(game)
{

}

ResourceManager::~ResourceManager()
{

}

Texture* ResourceManager::GetTexture(const std::string& fileName)
{
	Texture* tex = nullptr;
	auto iter = mTextures.find(fileName);
	if (iter != mTextures.end())
	{
		tex = iter->second.get();
	}
	else
	{
		std::unique_ptr<Texture> uniTex = std::make_unique<Texture>();
		if (uniTex->Load(fileName))
		{
			tex = uniTex.get();
			mTextures.emplace(fileName, std::move(uniTex));
		}
	}
	return tex;
}

Mesh* ResourceManager::GetMesh(const std::string& fileName)
{
	Mesh* m = nullptr;
	auto iter = mMeshes.find(fileName);
	if (iter != mMeshes.end())
	{
		m = iter->second.get();

	}
	else
	{
		std::unique_ptr<Mesh> ptr = std::make_unique<Mesh>();
		if (ptr->Load(fileName, mGame->GetRendererInstance()))
		{
			m = ptr.get();
			mMeshes.emplace(fileName, std::move(ptr));
		}
	}
	return m;
}

void ResourceManager::Unload()
{
	mTextures.clear();
}