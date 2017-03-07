#pragma once
#include "Shader.h"

class UIShader : public Shader
{
private:
	GLuint uniform_anchor;
	GLuint uniform_resolution;
	GLuint uniform_scale;

protected:
	virtual void Start() override;
	virtual void Stop() override;
	virtual bool Load() override;
	virtual void Render(CameraComponent* camera, ModelComponentBase* component, float frame_time) override;
	virtual void AttachShaders() override;
};

