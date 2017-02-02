#pragma once
#include "ModelComponentBase.h"

class ModelComponent : public ModelComponentBase
{
public:
	Model* model;
	Shader* shader;

	ModelComponent(Model* model = nullptr, Shader* shader = nullptr);

	virtual Model* GetModel() override { return model; };
	virtual Shader* GetShader() override { return shader; };
	virtual Transform GetTransform() override { return Transform(); };
};

