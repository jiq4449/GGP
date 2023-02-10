#pragma once
#include <memory>
#include "Mesh.h"
#include "Transform.h"
#include "Material.h"

class GameEntity
{
public:
	GameEntity(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material);

	// Getter Functions
	std::shared_ptr<Mesh> GetMesh();
	std::shared_ptr<Material> GetMaterial();
	Transform* GetTransform();

	// Setter Functions
	void SetMesh(std::shared_ptr<Mesh> mesh);
	void SetMaterial(std::shared_ptr<Material> material);

private:
	std::shared_ptr<Mesh> mesh;
	std::shared_ptr<Material> material;

	Transform transform;
};