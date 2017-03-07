#pragma once
#include "API.h"
#include "ModelComponentBase.h"

class POOT_API ModelComponent : public ModelComponentBase
{
public:
	Model* model;
	Shader* shader;

	virtual Model* GetModel() override { return model; };
	virtual Shader* GetShader() override { return shader; };
};

