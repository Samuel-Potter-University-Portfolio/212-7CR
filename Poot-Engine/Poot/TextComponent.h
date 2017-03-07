#pragma once
#include "ModelComponent.h"
#include <vec3.hpp>
#include <string>

class TextComponent : public ModelComponent
{
public:
	glm::vec3 colour;
	std::string text;
};
