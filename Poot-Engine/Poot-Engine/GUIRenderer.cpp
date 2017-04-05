#include "GUIRenderer.h"
#include "Game.h"


GUIRenderer::GUIRenderer()
{
	main_canvas.cleanup_children = false;
}

void GUIRenderer::HandleNewComponent(Component* component)
{
	Canvas* canvas_comp = Cast<Canvas>(component);

	if (canvas_comp)
		main_canvas.AddElement(canvas_comp);
}

void GUIRenderer::Render(RenderRequest& request)
{
	main_canvas.Render(request);
	glBindVertexArray(0);
}
