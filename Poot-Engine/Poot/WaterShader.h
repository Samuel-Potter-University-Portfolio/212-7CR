#pragma once
#include "Shader.h"


class WaterShader : public Shader
{
private:
	GLuint uniform_model_matrix;
	GLuint uniform_view_matrix;
	GLuint uniform_projection_matrix;
	GLuint uniform_camera_location;
	GLuint uniform_camera_near_plane;
	GLuint uniform_camera_far_plane;

	GLuint uniform_dudv_uv_offset0;
	GLuint uniform_dudv_uv_offset1;

	GLuint du_dv_map;

protected:
	virtual void Start() override;
	virtual void Stop() override;
	virtual bool Load() override;
	virtual void Render(CameraComponent* camera, ModelComponentBase* component, float frame_time) override;
	virtual void AttachShaders() override;
};

