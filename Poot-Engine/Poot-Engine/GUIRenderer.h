#pragma once
#include "Component.h"
#include "Canvas.h"

#include <vector>

class GUIRenderer
{
private:
	Canvas main_canvas;

public:
	GUIRenderer();

	void HandleNewComponent(Component* component);
	void Render(struct RenderRequest& request);
};

