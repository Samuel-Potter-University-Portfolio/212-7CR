#pragma once
#include "API.h"
#include "GameObject.h"
#include "InputComponent.h"
#include "Canvas.h"
#include "TextElement.h"


class POOT_API DebugPanel : public GameObject
{
private:
	InputComponent* input_component;
	Canvas* main_canvas;

public:
	DebugPanel();

	virtual void BuildComponents() override;
	virtual void Tick(float delta_time) override;

	void OnTogglePanel(bool pressed);
};

