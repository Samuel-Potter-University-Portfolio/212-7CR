#pragma once
#include "Shader.h"


class DefaultShader : public Shader
{
public:
	DefaultShader() : Shader("DefaultShader") {}

protected:
	virtual void AttachShaders() override;
};

