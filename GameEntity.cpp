#include "GameEntity.h"

using namespace DirectX;

GameEntity::GameEntity(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material)
{
    this->mesh = mesh;
    this->material = material;
}

std::shared_ptr<Mesh> GameEntity::GetMesh() { return mesh; }

std::shared_ptr<Material> GameEntity::GetMaterial() { return material; }

Transform* GameEntity::GetTransform() { return &transform; }

void GameEntity::SetMesh(std::shared_ptr<Mesh> mesh) { this->mesh = mesh; }

void GameEntity::SetMaterial(std::shared_ptr<Material> material) { this->material = material; }