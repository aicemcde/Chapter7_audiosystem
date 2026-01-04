#pragma once
#include <memory>
#include <unordered_map>
#include <string>

class Texture;

class ResourceManager
{
public:
	explicit ResourceManager(class Game* game);
	~ResourceManager();
	Texture* GetTexture(const std::string& fileName);
	class Mesh* GetMesh(const std::string& fileName);
	void Unload();
private:
	class Game* mGame;
	std::unordered_map<std::string, std::unique_ptr<Texture>> mTextures;
	std::unordered_map<std::string, std::unique_ptr<Mesh>> mMeshes;
};