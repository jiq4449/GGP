#pragma once
#include <memory>
#include "Mesh.h"
#include "Transform.h"

class GameEntity
{
public:
	GameEntity(std::shared_ptr<Mesh> mesh);

	// Getter Functions
	std::shared_ptr<Mesh> GetMesh();
	Transform* GetTransform();

	// Setter Functions
	void SetMesh(std::shared_ptr<Mesh> mesh);

private:
	std::shared_ptr<Mesh> mesh;

	Transform transform;
};