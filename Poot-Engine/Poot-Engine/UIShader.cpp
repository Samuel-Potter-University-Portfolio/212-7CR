#include "UIShader.h"
#include "GameObject.h"
#include "Game.h"
#include "QuadUI.h"

const char* ui_vert_source =
#include "Shaders/UI.vert.glsl"
;
const char* ui_frag_source =
#include "Shaders/UI.frag.glsl"
;


void UIShader::Start()
{
	glUseProgram(GetProgramID());
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glBindVertexArray(quad_model->GetVAO());
}

void UIShader::Stop() 
{
	glBindVertexArray(0);
	glEnable(GL_CULL_FACE);
	glDisable(GL_CULL_FACE);
	glUseProgram(0);
}

bool UIShader::Load()
{
	if (!__super::Load())
		return false;

	uniform_model_matrix = glGetUniformLocation(GetProgramID(), "model_matrix");
	uniform_view_matrix = glGetUniformLocation(GetProgramID(), "view_matrix");
	uniform_projection_matrix = glGetUniformLocation(GetProgramID(), "projection_matrix");

	uniform_sun_direction = glGetUniformLocation(GetProgramID(), "sun_direction");
	uniform_sun_colour = glGetUniformLocation(GetProgramID(), "sun_colour");

	uniform_shininess = glGetUniformLocation(GetProgramID(), "shininess");
	uniform_roughness = glGetUniformLocation(GetProgramID(), "roughness");

	//Register UI Quad
	{
		ModelMesh mesh(
		{
			-1.0, -1.0, 0.0,	-1.0, 1.0, 0.0,
			1.0, 1.0, 0.0,		1.0, -1.0, 0.0,
		},
		{
			0.0, 0.0, 0.0, 1.0,
			1.0, 1.0, 1.0, 0.0
		},
		{},
		{
			2,1,0, 3,2,0,
		}
		);
		g_game->GetWindow()->GetModelLoader().RegisterModel("ui_quad", mesh);
	}

	quad_model = GameObject::LoadModelAsset("ui_quad");
}

void UIShader::AmbiguousRender(const RenderRequest& request, Component* component, float frame_time)
{
	QuadUI* quad = Cast<QuadUI>(component);
	if (!quad)
		return;

	glBindVertexArray(quad_model->GetVAO());
	glDrawElements(GL_TRIANGLES, quad_model->GetIndexCount(), GL_UNSIGNED_INT, nullptr);
}
/*
void UIShader::Render(CameraComponent* camera, ModelComponentBase* component, float frame_time)
{
	DirectionalLightComponent* sun = g_game->GetWorld()->GetSunLight();

	const glm::vec3 sun_direction = sun ? sun->GetDirection() : glm::vec3(0);
	const glm::vec3 sun_colour = sun ? sun->colour : glm::vec3(0);

	glUniform3f(uniform_sun_direction, sun_direction.x, sun_direction.y, sun_direction.z);
	glUniform3f(uniform_sun_colour, sun_colour.x, sun_colour.y, sun_colour.z);

	glUniform1f(uniform_shininess, component->GetFloatUnit(SHADER_UNIT_SHININESS));
	glUniform1f(uniform_roughness, component->GetFloatUnit(SHADER_UNIT_ROUGHNESS));

	glUniformMatrix4fv(uniform_model_matrix, 1, GL_FALSE, &component->GetTransformationMatrix(frame_time)[0][0]);
	glUniformMatrix4fv(uniform_view_matrix, 1, GL_FALSE, camera ? &camera->GetViewMatrix(frame_time)[0][0] : &glm::mat4(1.0)[0][0]);
	glUniformMatrix4fv(uniform_projection_matrix, 1, GL_FALSE, camera ? &camera->GetProjectionMatrix()[0][0] : &glm::mat4(1.0)[0][0]);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, component->GetTextureUnit(0));
	__super::Render(camera, component, frame_time);
}
*/
void UIShader::AttachShaders()
{
	glAttachShader(GetProgramID(), CreateShader(GL_VERTEX_SHADER, ui_vert_source));
	glAttachShader(GetProgramID(), CreateShader(GL_FRAGMENT_SHADER, ui_frag_source));
}