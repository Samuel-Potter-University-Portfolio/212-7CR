#pragma once
#include "Shader.h"
#include "vec4.hpp"


class ClipShader : public Shader
{
private:
	GLuint uniform_model_matrix;
	GLuint uniform_view_matrix;
	GLuint uniform_projection_matrix;
	GLuint uniform_cliping_plane;

	GLuint uniform_sun_direction;
	GLuint uniform_sun_colour;

	GLuint uniform_shininess;
	GLuint uniform_roughness;

protected:
	virtual void Start() override;
	virtual void Stop() override;
	virtual bool Load() override;
	virtual void Render(CameraComponent* camera, ModelComponentBase* component, float frame_time) override;
	virtual void AttachShaders() override;

public:
	glm::vec4 plane;
};
