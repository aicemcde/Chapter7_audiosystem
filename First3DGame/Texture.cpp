#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <SDL.h>
#include <glew.h>
#include <SDL_ttf.h>

Texture::Texture()
	:mTextureID(0)
	, mWidth(0)
	, mHeight(0)
{

}

Texture::~Texture()
{

}

bool Texture::Load(const std::string& fileName)
{
	int channels = 0;

	unsigned char* image = stbi_load(fileName.c_str(), &mWidth, &mHeight, &channels, 0);

	if (!image)
	{
		SDL_Log("Failed to load image : %s", fileName.c_str());
		return false;
	}

	int format = GL_RGB;
	if (channels == 4)
	{
		format = GL_RGBA;
	}

	glGenTextures(1, &mTextureID);
	glBindTexture(GL_TEXTURE_2D, mTextureID);

	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		format,
		mWidth,
		mHeight,
		0,
		format,
		GL_UNSIGNED_BYTE,
		image
	);

	stbi_image_free(image);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return true;
}

void Texture::Unload()
{
	glDeleteTextures(1, &mTextureID);
}

void Texture::SetActive()
{
	glBindTexture(GL_TEXTURE_2D, mTextureID);
}

bool Texture::CreateFromText(const std::string& text, const std::string& fontName, int pointSize, const Vector3& color)
{
	TTF_Font* font = TTF_OpenFont(fontName.c_str(), pointSize);
	if (!font)
	{
		SDL_Log("Failed to load font : %s", fontName.c_str());
		return false;
	}

	SDL_Color sdlColor;
	sdlColor.r = static_cast<Uint8>(color.x * 255);
	sdlColor.g = static_cast<Uint8>(color.y * 255);
	sdlColor.b = static_cast<Uint8>(color.z * 255);
	sdlColor.a = 255;

	SDL_Surface* surface = TTF_RenderUTF8_Blended(font, text.c_str(), sdlColor);

	TTF_CloseFont(font);

	if (!surface)
	{
		return false;
	}

	SDL_Surface* formattedSurface = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_ABGR8888, 0);

	SDL_FreeSurface(surface);

	if (!formattedSurface)
	{
		SDL_Log("Failed to convert surface format");
		return false;
	}

	mWidth = formattedSurface->w;
	mHeight = formattedSurface->h;

	if (mTextureID != 0)
	{
		glDeleteTextures(1, &mTextureID);
	}

	glGenTextures(1, &mTextureID);
	glBindTexture(GL_TEXTURE_2D, mTextureID);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	int bytesPerPixel = formattedSurface->format->BytesPerPixel;
	int pitchPixels = formattedSurface->pitch;

	int format = (bytesPerPixel == 4) ? GL_RGBA : GL_RGB;

	glTexImage2D(GL_TEXTURE_2D, 0, format, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, formattedSurface->pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	SDL_FreeSurface(formattedSurface);

	return true;
}