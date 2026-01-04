#pragma once
#include "Component.h"

class MeshComponent : public Component
{
public:
	MeshComponent(class Actor* owner);
	~MeshComponent();

	virtual void Draw(class Shader* shader);
	virtual void SetMesh(class Mesh* mesh) { mMesh = mesh; };
	void SetTextureIndex(size_t index) { mTextureIndex = index; };
protected:
	class Mesh* mMesh;
	size_t mTextureIndex;
};