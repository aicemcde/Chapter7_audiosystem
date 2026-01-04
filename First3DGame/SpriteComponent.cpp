#include "SpriteComponent.h"
#include <glew.h>
#include "Math.h"
#include "Actor.h"
#include "Shader.h"
#include "Game.h"
#include "Renderer.h"
#include "Texture.h"

SpriteComponent::SpriteComponent(Actor* owner, int updateOrder)
	:Component(owner, updateOrder)
	, mTexHeight(0)
	, mTexWidth(0)
	, mTexture(nullptr)
{
	Game::GetRendererInstance()->AddSprite(this);
}

SpriteComponent::~SpriteComponent()
{
	
}

void SpriteComponent::Draw(Shader* shader)
{
	Matrix4 scaleMat = Matrix4::CreateScale(
		static_cast<float>(mTexWidth),
		static_cast<float>(mTexHeight),
		1.0f
	);
	Matrix4 world = scaleMat * mOwner->GetWorldTransform();
	shader->SetMatrixUniform("uWorldTransform", world);
	mTexture->SetActive();
	glDrawElements(
		GL_TRIANGLES,
		6,
		GL_UNSIGNED_INT,
		nullptr
	);
}

void SpriteComponent::SetTexture(Texture* texture)
{
	mTexture = texture;
	mTexWidth = mTexture->GetWidth();
	mTexHeight = mTexture->GetHeight();
}