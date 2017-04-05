#pragma once
#include <GameObject.h>
#include <ModelComponent.h>
#include <MeshCollider.h>


class PoolTable : public GameObject3D
{
private:
	ModelComponent* body_model;
	ModelComponent* surface_model;
	MeshCollider* mesh_collider;
	Mesh* mesh;

public:
	PoolTable();
	~PoolTable();

	virtual void BuildComponents() override;
};

