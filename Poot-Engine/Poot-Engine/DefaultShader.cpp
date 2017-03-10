#include "DefaultShader.h"
#include "Game.h"

const char* default_vert_source =
#include "Shaders/Default.vert.glsl"
;
const char* default_frag_source =
#include "Shaders/Default.frag.glsl"
;


void DefaultShader::Start() 
{
	Super::Start();
	glCullFace(GL_BACK);
}

bool DefaultShader::Load() 
{
	if (!Super::Load())
		return false;

	uniform_model_matrix = glGetUniformLocation(GetProgramID(), "model_matrix");
	uniform_view_matrix = glGetUniformLocation(GetProgramID(), "view_matrix");
	uniform_projection_matrix = glGetUniformLocation(GetProgramID(), "projection_matrix");

	uniform_sun_direction = glGetUniformLocation(GetProgramID(), "sun_direction");
	uniform_sun_colour = glGetUniformLocation(GetProgramID(), "sun_colour");

	uniform_shininess = glGetUniformLocation(GetProgramID(), "shininess");
	uniform_roughness = glGetUniformLocation(GetProgramID(), "roughness");
}

void DefaultShader::Render(const RenderRequest& request, ModelComponentBase* component)
{
	DirectionalLightComponent* sun = g_game->GetWorld()->GetSunLight();

	const glm::vec3 sun_direction = sun ? sun->GetDirection() : glm::vec3(0);
	const glm::vec3 sun_colour = sun ? sun->colour : glm::vec3(0);

	glUniform3f(uniform_sun_direction, sun_direction.x, sun_direction.y, sun_direction.z);
	glUniform3f(uniform_sun_colour, sun_colour.x, sun_colour.y, sun_colour.z);

	glUniform1f(uniform_shininess, component->GetFloatUnit(SHADER_UNIT_SHININESS));
	glUniform1f(uniform_roughness, component->GetFloatUnit(SHADER_UNIT_ROUGHNESS));

	glUniformMatrix4fv(uniform_model_matrix, 1, GL_FALSE, &component->GetTransformationMatrix()[0][0]);
	glUniformMatrix4fv(uniform_view_matrix, 1, GL_FALSE, request.camera ? &request.camera->GetViewMatrix()[0][0] : &glm::mat4(1.0)[0][0]);
	glUniformMatrix4fv(uniform_projection_matrix, 1, GL_FALSE, request.camera ? &request.camera->GetProjectionMatrix()[0][0] : &glm::mat4(1.0)[0][0]);
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, component->GetTextureUnit(0));
	Super::Render(request, component);
}

void DefaultShader::AttachShaders() 
{
	glAttachShader(GetProgramID(), CreateShader(GL_VERTEX_SHADER, default_vert_source));
	glAttachShader(GetProgramID(), CreateShader(GL_FRAGMENT_SHADER, default_frag_source));
}
