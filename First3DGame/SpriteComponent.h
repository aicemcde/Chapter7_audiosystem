#pragma once
#include "Component.h"
#include <SDL.h>

class Texture;

class SpriteComponent : public Component
{
public:
	SpriteComponent(class Actor* owner, int updateOrder = 20);
	~SpriteComponent();

	void Draw(class Shader* shader);
	void SetTexture(Texture* texture);
private:
	int mTexWidth;
	int mTexHeight;
	Texture* mTexture;
};