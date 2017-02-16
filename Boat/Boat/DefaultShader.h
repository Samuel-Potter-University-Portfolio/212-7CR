#pragma once
#include "Shader.h"
#include "CameraComponent.h"
#include "ModelComponentBase.h"

class DefaultShader : public Shader
{
private:
	GLuint uniform_model_matrix;
	GLuint uniform_view_matrix;
	GLuint uniform_projection_matrix;

	GLuint uniform_sun_direction;
	GLuint uniform_sun_colour;

protected:
	virtual void Start() override;
	virtual bool Load() override;
	virtual void Render(CameraComponent* camera, ModelComponentBase* component, float frame_time) override;
	virtual void AttachShaders() override;
};

