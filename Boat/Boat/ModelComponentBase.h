#pragma once
#include "Component.h"
#include "Entity.h"
#include "Model.h"
#include "Shader.h"
#include "Transform.h"

class ModelComponentBase : public Component
{
private:
	bool is_visable = true;

public:
	virtual Shader* GetShader() = 0;
	virtual Model* GetModel() = 0;
	virtual Transform GetTransform() = 0;

	inline const bool IsVisable() { return IsActive() && (parent ? parent->IsVisable() && is_visable : is_visable); }
	inline void SetVisable(const bool visable) { is_visable = visable; }
};

