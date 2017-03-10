#include "SkyboxShader.h"
#include "ModelComponentBase.h"
#include "CameraComponent.h"

const char* skybox_vert_source =
#include "Shaders/Skybox.vert.glsl"
;
const char* skybox_frag_source =
#include "Shaders/Skybox.frag.glsl"
;


void SkyboxShader::Start()
{
	__super::Start();
	glCullFace(GL_BACK);
}

bool SkyboxShader::Load()
{
	if (!__super::Load())
		return false;

	uniform_camera_position = glGetUniformLocation(GetProgramID(), "camera_position");
	uniform_view_matrix = glGetUniformLocation(GetProgramID(), "view_matrix");
	uniform_projection_matrix = glGetUniformLocation(GetProgramID(), "projection_matrix");
}

void SkyboxShader::Render(const RenderRequest& request, ModelComponentBase* component, float frame_time)
{
	glm::vec3 camera_loc = request.camera ? request.camera->GetWorldLocation(frame_time) : glm::vec3(0);

	glUniform3f(uniform_camera_position, camera_loc.x, camera_loc.y, camera_loc.z);
	glUniformMatrix4fv(uniform_view_matrix, 1, GL_FALSE, request.camera ? &request.camera->GetViewMatrix(frame_time)[0][0] : &glm::mat4(1.0)[0][0]);
	glUniformMatrix4fv(uniform_projection_matrix, 1, GL_FALSE, request.camera ? &request.camera->GetProjectionMatrix()[0][0] : &glm::mat4(1.0)[0][0]);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, component->GetTextureUnit(0));
	__super::Render(request, component, frame_time);
}

void SkyboxShader::AttachShaders()
{
	glAttachShader(GetProgramID(), CreateShader(GL_VERTEX_SHADER, skybox_vert_source));
	glAttachShader(GetProgramID(), CreateShader(GL_FRAGMENT_SHADER, skybox_frag_source));
}
