#include "DebugPanel.h"
#include "QuadUI.h"


DebugPanel::DebugPanel()
{
	SetTags(OBJ_TAG_UI);

	input_component = MakeComponent<InputComponent>();

	main_canvas = MakeComponent<Canvas>();

	{
		Canvas* canvas = main_canvas->MakeElement<Canvas>();
		canvas->canvas_mode = Scaled;

		QuadUI* quad = canvas->MakeElement<QuadUI>();
		quad->texture_key = "Resources/Test_Image.png";
		quad->local_transform.scale *= 200;
		quad->local_transform.location = glm::vec2(200, -200);
		quad->anchor = glm::vec2(-1, 1);
	}
	{
		Canvas* canvas = main_canvas->MakeElement<Canvas>();
		canvas->canvas_mode = PixelPerfect;

		QuadUI* quad = canvas->MakeElement<QuadUI>();
		quad->texture_key = "Resources/Test_Image.png";
		quad->local_transform.scale *= 200;
		quad->local_transform.location = glm::vec2(-200, 200);
		quad->anchor = glm::vec2(1, -1);
		quad->colour = glm::vec4(1, 0, 1, 0.5f);

		TextElement* text = canvas->MakeElement<TextElement>();
		text->text = "This is a test!";
		text->font_sheet_key = "Resources/arial_ascii_bitmap.bmp";
		text->local_transform.scale *= 50;
		text->anchor = glm::vec2(0, 0);
		text->colour = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	}
}

void DebugPanel::BuildComponents()
{
	//Register toggle panel
	{
		InputEvent crouch_event;
		crouch_event.AddInput(GLFW_KEY_F1);
		crouch_event.func = [this](bool pressed) { OnTogglePanel(pressed); };
		input_component->AddEvent(crouch_event);
	}
}

void DebugPanel::Tick(float delta_time)
{
	Super::Tick(delta_time);

	if (!main_canvas->IsActive())
		return;

	//Update values
}

void DebugPanel::OnTogglePanel(bool pressed) 
{
	if (pressed)
		main_canvas->SetActive(!main_canvas->IsActive());
}