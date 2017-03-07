#pragma once
#include "API.h"
#include "Shader.h"

class POOT_API SkyboxShader : public Shader
{
private:
	GLuint uniform_camera_position;
	GLuint uniform_view_matrix;
	GLuint uniform_projection_matrix;

protected:
	virtual void Start() override;
	virtual bool Load() override;
	virtual void Render(CameraComponent* camera, ModelComponentBase* component, float frame_time) override;
	virtual void AttachShaders() override;
};

