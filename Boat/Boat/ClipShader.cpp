#include "ClipShader.h"
#include "Game.h"

const char* clip_vert_source =
#include "Shaders/Clip.vert.glsl"
;
const char* clip_frag_source =
#include "Shaders/Clip.frag.glsl"
;


void ClipShader::Start()
{
	__super::Start();
	glCullFace(GL_BACK);
	glEnable(GL_CLIP_DISTANCE0);
}

void ClipShader::Stop() 
{
	glDisable(GL_CLIP_DISTANCE0);
	__super::Stop();
}

bool ClipShader::Load()
{
	if (!__super::Load())
		return false;

	uniform_model_matrix = glGetUniformLocation(GetProgramID(), "model_matrix");
	uniform_view_matrix = glGetUniformLocation(GetProgramID(), "view_matrix");
	uniform_projection_matrix = glGetUniformLocation(GetProgramID(), "projection_matrix");
	uniform_cliping_plane = glGetUniformLocation(GetProgramID(), "cliping_plane");
}

void ClipShader::Render(CameraComponent* camera, ModelComponentBase* component, float frame_time)
{
	glUniformMatrix4fv(uniform_model_matrix, 1, GL_FALSE, &component->GetTransformationMatrix(frame_time)[0][0]);
	glUniformMatrix4fv(uniform_view_matrix, 1, GL_FALSE, camera ? &camera->GetViewMatrix(frame_time)[0][0] : &glm::mat4(1.0)[0][0]);
	glUniformMatrix4fv(uniform_projection_matrix, 1, GL_FALSE, camera ? &camera->GetProjectionMatrix()[0][0] : &glm::mat4(1.0)[0][0]);
	glUniform4f(uniform_cliping_plane, plane.x, plane.y, plane.z, plane.w);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, component->GetTextureUnit(0));
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, component->GetTextureUnit(1));

	__super::Render(camera, component, frame_time);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void ClipShader::AttachShaders()
{
	glAttachShader(GetProgramID(), CreateShader(GL_VERTEX_SHADER, clip_vert_source));
	glAttachShader(GetProgramID(), CreateShader(GL_FRAGMENT_SHADER, clip_frag_source));
}