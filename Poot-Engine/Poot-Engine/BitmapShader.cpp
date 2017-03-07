#include "BitmapShader.h"
#include "TextComponent.h"
#include "Game.h"

const char* bitmap_vert_source =
#include "Shaders/Bitmap.vert.glsl"
;
const char* bitmap_frag_source =
#include "Shaders/Bitmap.frag.glsl"
;

void BitmapShader::Start()
{
	__super::Start();
	glDisable(GL_CULL_FACE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void BitmapShader::Stop()
{
	glDisable(GL_BLEND);
	glEnable(GL_CULL_FACE);
	__super::Stop();
}

bool BitmapShader::Load()
{
	if (!__super::Load())
		return false;

	uniform_location = glGetUniformLocation(GetProgramID(), "location");

	uniform_anchor = glGetUniformLocation(GetProgramID(), "anchor");
	uniform_resolution = glGetUniformLocation(GetProgramID(), "resolution");
	uniform_scale = glGetUniformLocation(GetProgramID(), "scale");

	uniform_colour = glGetUniformLocation(GetProgramID(), "colour");
	uniform_char_index = glGetUniformLocation(GetProgramID(), "char_index");
}

void BitmapShader::Render(CameraComponent* camera, ModelComponentBase* component, float frame_time)
{
	TextComponent* text_comp = (TextComponent*)component;
	if (!text_comp)
		return;

	if (camera)
		glUniform2f(uniform_resolution, g_game->GetWindow()->GetWidth(), g_game->GetWindow()->GetHeight());
	else
		glUniform2f(uniform_resolution, 100, 100);

	const glm::vec3 anchor = component->transform.GetLerpLocation(frame_time);
	glUniform3f(uniform_anchor, anchor.x, anchor.y, anchor.z);
	const glm::vec3 scale = component->transform.GetLerpScale(frame_time);
	glUniform3f(uniform_scale, scale.x, scale.y, scale.z);

	glUniform3f(uniform_colour, text_comp->colour.x, text_comp->colour.y, text_comp->colour.z);
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, component->GetTextureUnit(0));


	float text_x = 0;
	float text_y = 0;
	for (char c : text_comp->text)
	{
		if (c == ' ')
		{
			text_x++;
			continue;
		}
		if (c == '\n')
		{
			text_x = 0;
			text_y--;
			continue;
		}
		if (c == '\t')
		{
			text_x += 2;
			continue;
		}


		glUniform2f(uniform_location, text_x, text_y);
		glUniform1i(uniform_char_index, c);
		__super::Render(camera, component, frame_time);
		text_x++;
	}
}

void BitmapShader::AttachShaders()
{
	glAttachShader(GetProgramID(), CreateShader(GL_VERTEX_SHADER, bitmap_vert_source));
	glAttachShader(GetProgramID(), CreateShader(GL_FRAGMENT_SHADER, bitmap_frag_source));
}
