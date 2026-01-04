#include "MeshComponent.h"
#include "Shader.h"
#include "Actor.h"
#include "Texture.h"
#include "Mesh.h"
#include "VertexArray.h"
#include <glew.h>
#include <SDL.h>
#include "Game.h"
#include "Renderer.h"

MeshComponent::MeshComponent(Actor* owner)
	:Component(owner)
	, mMesh(nullptr)
	, mTextureIndex(0)
{
	Game::GetRendererInstance()->AddMeshComp(this);
}

MeshComponent::~MeshComponent()
{

}

void MeshComponent::Draw(Shader* shader)
{
	if (mMesh)
	{
		shader->SetMatrixUniform("uWorldTransform",
			mOwner->GetWorldTransform());
		shader->SetFloatUniform("uSpecPower", mMesh->GetSpecPower());
		Texture* t = mMesh->GetTexture(mTextureIndex);
		if (t)
		{ 
			t->SetActive();
		}
		else
		{
			SDL_Log("MeshComponent : Texture does not get");
		}
		VertexArray* va = mMesh->GetVertexArray();
		SDL_assert(va != nullptr);
		va->SetActive();
		glDrawElements(GL_TRIANGLES, va->GetNumIndices(),
			GL_UNSIGNED_INT, nullptr);
	}
	else
	{
		SDL_Log("Mesh could not draw");
	}
}