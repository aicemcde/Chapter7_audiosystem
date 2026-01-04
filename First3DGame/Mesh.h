#pragma once
#include <string>
#include <vector>
#include <memory>

class Mesh
{
public:
	Mesh();
	~Mesh();

	bool Load(const std::string& fileName, class Renderer* renderer);
	void Unload();

	class VertexArray* GetVertexArray() { return mVertexArray.get(); }
	class Texture* GetTexture(size_t index);
	const std::string& GetShaderName() const { return mShaderName; }
	float GetRadius() const { return mRadius; }
	float GetSpecPower() const noexcept { return mSpecPower; }
private:
	std::vector<class Texture*> mTextures;
	std::unique_ptr<class VertexArray> mVertexArray;
	std::string mShaderName;
	float mRadius;
	float mSpecPower;
};