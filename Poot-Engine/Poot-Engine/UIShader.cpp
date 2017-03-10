#include "UIShader.h"
#include "GameObject.h"
#include "Game.h"
#include "QuadUI.h"
#include "Canvas.h"

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

	if(quad_model)
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
	uniform_frame_size = glGetUniformLocation(GetProgramID(), "frame_size");
	uniform_anchor = glGetUniformLocation(GetProgramID(), "anchor");

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
	if (!quad_model)
		return;

	QuadUI* quad = Cast<QuadUI>(component);
	if (!quad)
		return;
	glBindVertexArray(quad_model->GetVAO());

	//Set frame size based on canvas mode
	Canvas* canvas = quad->parent_canvas;
	if (canvas && canvas->canvas_mode == Scaled)
		glUniform2f(uniform_frame_size, 400.0f * request.frame_width / request.frame_height, 400.0f);

	else
		glUniform2f(uniform_frame_size, request.frame_width, request.frame_height);


	glUniform2f(uniform_anchor, quad->anchor.x, quad->anchor.y);
	glUniformMatrix3fv(uniform_model_matrix, 1, GL_FALSE, &quad->GetTransformationMatrix(frame_time)[0][0]);


	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, quad->texture);

	glDrawElements(GL_TRIANGLES, quad_model->GetIndexCount(), GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
}

void UIShader::AttachShaders()
{
	glAttachShader(GetProgramID(), CreateShader(GL_VERTEX_SHADER, ui_vert_source));
	glAttachShader(GetProgramID(), CreateShader(GL_FRAGMENT_SHADER, ui_frag_source));
}
