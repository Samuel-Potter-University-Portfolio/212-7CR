#include "WaterShader.h"
#include "Game.h"

const char* water_vert_source =
#include "Shaders/Water.vert.glsl"
;
const char* water_frag_source =
#include "Shaders/Water.frag.glsl"
;


void WaterShader::Start()
{
	__super::Start();
	glCullFace(GL_BACK);
	//Uncomment for alt. blending
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void WaterShader::Stop() 
{
	//glDisable(GL_BLEND);
	__super::Stop();
}

bool WaterShader::Load()
{
	if (!__super::Load())
		return false;

	uniform_model_matrix = glGetUniformLocation(GetProgramID(), "model_matrix");
	uniform_view_matrix = glGetUniformLocation(GetProgramID(), "view_matrix");
	uniform_projection_matrix = glGetUniformLocation(GetProgramID(), "projection_matrix");

	uniform_camera_location = glGetUniformLocation(GetProgramID(), "camera_location");
	uniform_camera_near_plane = glGetUniformLocation(GetProgramID(), "camera_near_plane");
	uniform_camera_far_plane = glGetUniformLocation(GetProgramID(), "camera_far_plane");

	uniform_dudv_uv_offset0 = glGetUniformLocation(GetProgramID(), "dudv_uv_offset0");
	uniform_dudv_uv_offset1 = glGetUniformLocation(GetProgramID(), "dudv_uv_offset1");

	du_dv_map = g_game->GetWindow()->GetTextureLoader()["Resources/waterDUDV.png"];
}

void WaterShader::Render(CameraComponent* camera, ModelComponentBase* component, float frame_time)
{
	glUniformMatrix4fv(uniform_model_matrix, 1, GL_FALSE, &component->GetTransformationMatrix(frame_time)[0][0]);
	glUniformMatrix4fv(uniform_view_matrix, 1, GL_FALSE, camera ? &camera->GetViewMatrix(frame_time)[0][0] : &glm::mat4(1.0)[0][0]);
	glUniformMatrix4fv(uniform_projection_matrix, 1, GL_FALSE, camera ? &camera->GetProjectionMatrix()[0][0] : &glm::mat4(1.0)[0][0]);

	//Use 2 waves to simulate current
	{
		const float speed = 0.07f;
		glm::vec2 wave_direction = glm::normalize(glm::vec2(3, -2));
		wave_direction *= glfwGetTime() * speed;
		glUniform2f(uniform_dudv_uv_offset0, wave_direction.x, wave_direction.y);
	}
	{
		const float speed = 0.12f;
		glm::vec2 wave_direction = glm::normalize(glm::vec2(-4, 7));
		wave_direction *= glfwGetTime() * speed;
		glUniform2f(uniform_dudv_uv_offset1, wave_direction.x, wave_direction.y);
	}

	if (camera)
	{
		glm::vec3 camera_location = camera->transform.GetLerpLocation(frame_time);
		if (camera->GetParent())
			camera_location += camera->GetParent()->transform.GetLerpLocation(frame_time);

		glUniform1f(uniform_camera_near_plane, camera->near_plane);
		glUniform1f(uniform_camera_far_plane, camera->far_plane);
		glUniform3f(uniform_camera_location, camera_location.x, camera_location.y, camera_location.z);
	}

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, component->GetTextureUnit(0));
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, component->GetTextureUnit(1));
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, component->GetTextureUnit(2));
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, du_dv_map);

	__super::Render(camera, component, frame_time);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void WaterShader::AttachShaders()
{
	glAttachShader(GetProgramID(), CreateShader(GL_VERTEX_SHADER, water_vert_source));
	glAttachShader(GetProgramID(), CreateShader(GL_FRAGMENT_SHADER, water_frag_source));
}
