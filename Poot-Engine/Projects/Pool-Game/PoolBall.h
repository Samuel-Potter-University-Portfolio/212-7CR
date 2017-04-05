#pragma once
#include <GameObject.h>
#include <ModelComponent.h>
#include <Body.h>
#include <SphereCollider.h>

class PoolBall : public GameObject3D
{
private:
	ModelComponent* model;
	SphereCollider* sphere;
	Body* body;
	int number;
	bool potted = false;

public:
	PoolBall(int number);

	virtual void BuildComponents() override;
	virtual void Tick(float delta_time) override;

	inline int GetNumber() { return number; }
	inline bool IsStill() { return !body->IsAwake(); }

	virtual void OnCollide(class Collider* collider, struct HitInfo& hit_info) override;

	void Reset();
};

