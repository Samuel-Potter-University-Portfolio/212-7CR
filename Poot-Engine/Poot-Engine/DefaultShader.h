#pragma once
#include "API.h"
#include "Shader.h"
#include "CameraComponent.h"
#include "ModelComponentBase.h"

#define SHADER_UNITF_SHININESS 0
#define SHADER_UNITF_ROUGHNESS 1
#define SHADER_UNITF_REFLECTIVENESS 2

#define SHADER_UNITI_USING_PHONG_MAP 0

#define SHADER_UNITT_BASE_TEXTURE 0
#define SHADER_UNITT_PHONG_MAP 1 
#define SHADER_UNITT_REFLECTION_CM 2

class POOT_API DefaultShader : public Shader
{
private:
	GLuint uniform_model_matrix;
	GLuint uniform_view_matrix;
	GLuint uniform_projection_matrix;

	GLuint uniform_sun_direction;
	GLuint uniform_sun_colour;

	GLuint uniform_shininess;
	GLuint uniform_roughness;
	GLuint uniform_reflectiveness;

	GLuint uniform_using_phong_map;

protected:
	virtual void Start() override;
	virtual bool Load() override;
	virtual void Render(const RenderRequest& request, ModelComponentBase* component) override;
	virtual void AttachShaders() override;
};

