#include "UIShader.h"
#include "Game.h"

const char* ui_vert_source =
#include "Shaders/UI.vert.glsl"
;
const char* ui_frag_source =
#include "Shaders/UI.frag.glsl"
;


void UIShader::Start()
{
	__super::Start();
	glDisable(GL_CULL_FACE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void UIShader::Stop() 
{
	glDisable(GL_BLEND);
	glEnable(GL_CULL_FACE);
	__super::Stop();
}

bool UIShader::Load()
{
	if (!__super::Load())
		return false;

	uniform_anchor = glGetUniformLocation(GetProgramID(), "anchor");
	uniform_resolution = glGetUniformLocation(GetProgramID(), "resolution");
	uniform_scale = glGetUniformLocation(GetProgramID(), "scale");
}

void UIShader::Render(CameraComponent* camera, ModelComponentBase* component, float frame_time)
{
	if (camera)
		glUniform2f(uniform_resolution, g_game->GetWindow()->GetWidth(), g_game->GetWindow()->GetHeight());
	else
		glUniform2f(uniform_resolution, 100, 100);

	const glm::vec3 anchor = component->transform.GetLerpLocation(frame_time);
	glUniform3f(uniform_anchor, anchor.x, anchor.y, anchor.z);
	const glm::vec3 scale = component->transform.GetLerpScale(frame_time);
	glUniform3f(uniform_scale, scale.x, scale.y, scale.z);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, component->GetTextureUnit(0));
	__super::Render(camera, component, frame_time);
}

void UIShader::AttachShaders()
{
	glAttachShader(GetProgramID(), CreateShader(GL_VERTEX_SHADER, ui_vert_source));
	glAttachShader(GetProgramID(), CreateShader(GL_FRAGMENT_SHADER, ui_frag_source));
}
