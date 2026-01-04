#pragma once
#include <string>
#include "Math.h"

class Texture
{
public:
	Texture();
	~Texture();

	bool Load(const std::string& fileName);
	void Unload();

	bool CreateFromText(const std::string& text, const std::string& fontName, int pointSize, const Vector3& color);

	void SetActive();

	int GetWidth() const { return mWidth; }
	int GetHeight() const { return mHeight; }
private:
	unsigned int mTextureID;
	int mWidth;
	int mHeight;
};