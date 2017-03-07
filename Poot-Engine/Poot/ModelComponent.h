#pragma once
#include "ModelComponentBase.h"

class ModelComponent : public ModelComponentBase
{
public:
	Model* model;
	Shader* shader;

	virtual Model* GetModel() override { return model; };
	virtual Shader* GetShader() override { return shader; };
};

