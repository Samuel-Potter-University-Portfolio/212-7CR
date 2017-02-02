#pragma once
#include "Shader.h"
#include "ModelComponentBase.h"

class DefaultShader : public Shader
{
private:
	GLuint uniform_model_matrix;
	GLuint uniform_view_matrix;
	GLuint uniform_projection_matrix;

public:
	DefaultShader() : Shader("DefaultShader") {}

protected:
	virtual bool Load() override;
	virtual void Render(ModelComponentBase* component, float frame_time) override;
	virtual void AttachShaders() override;
};

