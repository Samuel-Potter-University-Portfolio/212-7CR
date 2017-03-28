#pragma once
#include <GameObject.h>
#include <ModelComponent.h>
#include <MeshCollider.h>


#define POBJ_TAG_BALL (OBJ_TAG_LAST << 1)
#define POBJ_TAG_TABLE (OBJ_TAG_LAST << 2)

#define POBJ_TAG_LAST OBJ_TAG_TABLE


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

