#include "BitmapShader.h"
#include "GameObject.h"
#include "Game.h"
#include "TextElement.h"
#include "Canvas.h"

const char* bitmap_vert_source =
#include "Shaders/Bitmap_Font.vert.glsl"
;
const char* bitmap_frag_source =
#include "Shaders/Bitmap_Font.frag.glsl"
;


void BitmapShader::Start()
{
	glUseProgram(GetProgramID());
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);

	if (quad_model)
		glBindVertexArray(quad_model->GetVAO());
}

void BitmapShader::Stop()
{
	glBindVertexArray(0);
	glEnable(GL_CULL_FACE);
	glDisable(GL_CULL_FACE);
	glUseProgram(0);
}

bool BitmapShader::Load()
{
	if (!Super::Load())
		return false;

	uniform_model_matrix = glGetUniformLocation(GetProgramID(), "model_matrix");
	uniform_frame_size = glGetUniformLocation(GetProgramID(), "frame_size");

	uniform_anchor = glGetUniformLocation(GetProgramID(), "anchor");
	uniform_colour = glGetUniformLocation(GetProgramID(), "colour");

	uniform_current_char = glGetUniformLocation(GetProgramID(), "current_char");
	uniform_letter_location = glGetUniformLocation(GetProgramID(), "letter_location");


	//Register character quad
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
		g_game->GetWindow()->GetModelLoader().RegisterModel("font_quad", mesh);
	}

	quad_model = GameObject::LoadModelAsset("font_quad");
}

void BitmapShader::AmbiguousRender(const RenderRequest& request, Component* component, float frame_time)
{
	if (!quad_model)
		return;

	TextElement* text = Cast<TextElement>(component);
	if (!text)
		return;
	glBindVertexArray(quad_model->GetVAO());

	//Set frame size based on canvas mode
	Canvas* canvas = text->parent_canvas;
	if (canvas && canvas->canvas_mode == Scaled)
		glUniform2f(uniform_frame_size, 400.0f * request.frame_width / request.frame_height, 400.0f);

	else
		glUniform2f(uniform_frame_size, request.frame_width, request.frame_height);


	glUniform2f(uniform_anchor, text->anchor.x, text->anchor.y);
	glUniform4f(uniform_colour, text->colour.r, text->colour.g, text->colour.b, text->colour.a);
	glUniformMatrix3fv(uniform_model_matrix, 1, GL_FALSE, &text->GetTransformationMatrix(frame_time)[0][0]);


	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, text->GetTextureID());

	glm::vec2 letter_location(0);
	std::string message = text->text;

	if(text->aligment == Left)
	{
		for (char& c : message)
			DrawLetter(c, letter_location, 1);
	}
	else
	{
		std::string current_sentence = "";

		for (char& c : message)
		{
			if (c == '\n')
			{
				for (auto it = current_sentence.rbegin(); it != current_sentence.rend(); ++it)
					DrawLetter(*it, letter_location, -1);
				current_sentence = "";
				DrawLetter('\n', letter_location, -1);
			}
			else
				current_sentence += c;
		}

		if(current_sentence.size() != 0)
			for (auto it = current_sentence.rbegin(); it != current_sentence.rend(); ++it)
				DrawLetter(*it, letter_location, -1);
	}

	glBindVertexArray(0);
}

void BitmapShader::DrawLetter(const char& c, glm::vec2& letter_location, const int alignment)
{
	const float spacing = 1.0f;

	if (c == '\n')
	{
		letter_location.x = 0;
		letter_location.y -= 1.8f;
	}
	else if (c == ' ')
	{
		letter_location.x += alignment * spacing;
	}
	else if (c == '\t')
	{
		letter_location.x += 3 * alignment * spacing;
	}
	else
	{
		glUniform1i(uniform_current_char, (int)c);
		glUniform2f(uniform_letter_location, letter_location.x, letter_location.y);
		glDrawElements(GL_TRIANGLES, quad_model->GetIndexCount(), GL_UNSIGNED_INT, nullptr);
		letter_location.x+= alignment * spacing;
	}
}

void BitmapShader::AttachShaders()
{
	glAttachShader(GetProgramID(), CreateShader(GL_VERTEX_SHADER, bitmap_vert_source));
	glAttachShader(GetProgramID(), CreateShader(GL_FRAGMENT_SHADER, bitmap_frag_source));
}
