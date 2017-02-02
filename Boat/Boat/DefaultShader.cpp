#include "DefaultShader.h"
#include "Logger.h"

const char* vert_source =
#include "Shaders/Default.vert.glsl"
;
const char* frag_source =
#include "Shaders/Default.frag.glsl"
;


bool DefaultShader::Load() 
{
	if (!__super::Load())
		return false;


	uniform_model_matrix = glGetUniformLocation(GetProgramID(), "model_matrix");
	uniform_view_matrix = glGetUniformLocation(GetProgramID(), "view_matrix");
	uniform_projection_matrix = glGetUniformLocation(GetProgramID(), "projection_matrix");
}

void DefaultShader::Render(ModelComponentBase* component, float frame_time)
{
	glUniformMatrix4fv(uniform_model_matrix, 1, GL_FALSE, &component->GetTransform().GetMatrix(frame_time)[0][0]);
	glUniformMatrix4fv(uniform_view_matrix, 1, GL_FALSE, &glm::mat4(1.0)[0][0]);
	glUniformMatrix4fv(uniform_projection_matrix, 1, GL_FALSE, &glm::mat4(1.0)[0][0]);
	__super::Render(component, frame_time);
}

void DefaultShader::AttachShaders() 
{
	glAttachShader(GetProgramID(), CreateShader(GL_VERTEX_SHADER, vert_source));
	glAttachShader(GetProgramID(), CreateShader(GL_FRAGMENT_SHADER, frag_source));
}
