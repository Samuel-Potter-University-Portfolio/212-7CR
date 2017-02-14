#pragma once
#include "PlaneCollider.h"

class SoftPlaneCollider : public PlaneCollider
{
protected:
	virtual void ResolveCollision(BaseBodyComponent* attached_body, HitInfo& hit_info) override;
};

