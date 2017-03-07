#pragma once
#include "API.h"
#include "Component.h"
#include "Tickable.h"

#include <vector>

class POOT_API UIElement : public Tickable
{
private:
	class World* world;
	std::vector<Component*> components;
	bool is_visable = true;

public:
	UIElement();
	~UIElement();
};

