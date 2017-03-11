#include "DebugPanel.h"
#include "QuadUI.h"
#include "Game.h"

#include <sstream>

DebugPanel::DebugPanel()
{
	SetTags(OBJ_TAG_UI);

	input_component = MakeComponent<InputComponent>();

	main_canvas = MakeComponent<Canvas>();
	main_canvas->sorting_layer = 100000;
#ifndef _DEBUG
	main_canvas->SetActive(false);
#endif

	//Add water mark stuff (Separate from main canvas
	{
		Canvas* canvas = MakeComponent<Canvas>();
		canvas->canvas_mode = PixelPerfect;

		//Poot engine label
		{
			std::stringstream message;
			message << "Poot Engine " << POOT_VERSION_MAJOR << '.' << POOT_VERSION_MINOR << '.' << POOT_VERSION_PATCH << '\n';
			message << "Built " << __DATE__ << " " << __TIME__ << '\n';
#ifdef _DEBUG
			message << "Debug Build\n";
#else
			message << "Release Build\n";
#endif

			TextElement* text = canvas->MakeElement<TextElement>();
			text->font_sheet_key = "Resources/arial_ascii_bitmap.bmp";
			text->text = message.str();
			text->local_transform.scale *= 20;
			text->local_transform.location = glm::vec2(-20, -20);
			text->anchor = glm::vec2(1, 1);
			text->aligment = Right;
			text->colour = glm::vec4(2.0f, 2.0f, 2.0f, 1.0f);
		}
	}

	//Add stats text
	{
		Canvas* canvas = main_canvas->MakeElement<Canvas>();
		canvas->canvas_mode = Scaled;

		//Poot engine label
		{
			stats_text = canvas->MakeElement<TextElement>();
			stats_text->font_sheet_key = "Resources/arial_ascii_bitmap.bmp";
			stats_text->local_transform.scale *= 20;
			stats_text->local_transform.location = glm::vec2(20, -20);
			stats_text->anchor = glm::vec2(-1, 1);
			stats_text->aligment = Left;
			stats_text->colour = glm::vec4(2.0f, 2.0f, 2.0f, 1.0f);
		}
	}


	/*
	QuadUI* quad = canvas->MakeElement<QuadUI>();
	quad->texture_key = "Resources/Test_Image.png";
	quad->local_transform.scale *= 200;
	quad->local_transform.location = glm::vec2(200, -200);
	quad->anchor = glm::vec2(-1, 1);
	*/
}

void DebugPanel::BuildComponents()
{
	//Register toggle panel
	{
		InputEvent event;
		event.AddInput(GLFW_KEY_F1);
		event.func = [this](bool pressed) { OnTogglePanel(pressed); };
		input_component->AddEvent(event);
	}
	//Register toggle panel
	{
		InputEvent event;
		event.AddInput(GLFW_KEY_F11);
		event.func = 
			[](bool pressed) 
			{ 
				if (pressed)
					g_game->GetWindow()->SetFullscreen(!g_game->GetWindow()->IsFullscreen());
			};
		input_component->AddEvent(event);
	}
}

void DebugPanel::Tick(float delta_time)
{
	Super::Tick(delta_time);

	if (!main_canvas->IsActive())
		return;

	//Update values
	std::stringstream message;
	message << "[F1] to toggle debug menu\n";
	message << "FPS: " << g_game->GetWindow()->GetCurrentTickRate() << '\n';
	message << "UPS: " << g_game->GetGameLogic()->GetCurrentTickRate() << '\n';
	message << "Time: " << (int)(glfwGetTime()) << '\n';
	message << "Objects: " << GetWorld()->GetAllObjects().size() << '\n';
	message << "\n[MasterRenderer]\n" << GetWorld()->GetMasterRenderer().GetStatusString() << '\n';
	message << "\n[PhysicsScene]\n" << GetWorld()->GetPhysicsScene().GetStatusString() << '\n';

	stats_text->text = message.str();
}

void DebugPanel::OnTogglePanel(bool pressed) 
{
	if (pressed)
		main_canvas->SetActive(!main_canvas->IsActive());
}