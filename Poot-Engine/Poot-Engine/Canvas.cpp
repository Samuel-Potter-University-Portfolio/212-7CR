#include "Canvas.h"



Canvas::Canvas()
{
}


Canvas::~Canvas()
{
	if(cleanup_children)
		for (Element* element : elements)
			delete element;
}


void Canvas::LogicTick(float delta_time)
{
	Super::LogicTick(delta_time);

	for (Element* element : elements)
		element->HandleLogicTick(delta_time);
}

void Canvas::WindowTick(float delta_time)
{
	Super::WindowTick(delta_time);

	for (Element* element : elements)
		element->HandleWindowTick(delta_time);
}

void Canvas::AddElement(Element* element) 
{
	Element* current_element = element;

	//Store in layer order
	for (int i = 0; i < elements.size(); ++i)
	{
		if (current_element->sorting_layer < elements[i]->sorting_layer)
		{
			Element* temp = elements[i];
			elements[i] = current_element;
			current_element = temp;
		}
	}

	elements.push_back(current_element);
	element->parent_canvas = this;
}

void Canvas::Render(RenderRequest& request)
{
	if (!IsEnabled())
		return;

	for (Element* element : elements)
		element->Render(request);
}