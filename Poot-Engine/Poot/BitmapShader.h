#pragma once
#include "API.h"
#include "Shader.h"

class POOT_API BitmapShader : public Shader
{
private:
	GLuint uniform_location;

	GLuint uniform_anchor;
	GLuint uniform_resolution;
	GLuint uniform_scale;

	GLuint uniform_colour;
	GLuint uniform_char_index;

protected:
	virtual void Start() override;
	virtual void Stop() override;
	virtual bool Load() override;
	virtual void Render(CameraComponent* camera, ModelComponentBase* component, float frame_time) override;
	virtual void AttachShaders() override;
};

