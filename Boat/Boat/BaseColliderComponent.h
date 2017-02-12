#pragma once
#include "Component.h"

class BaseColliderComponent : public Component
{
protected:
	class BaseBodyComponent* attached_body;

public:

	inline BaseBodyComponent* GetAttachedBody() { return attached_body; }
	inline void SetAttachedBody(BaseBodyComponent* attached_body) { this->attached_body = attached_body; }
};

