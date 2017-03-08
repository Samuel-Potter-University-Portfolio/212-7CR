#pragma once
#include "API.h"
#include "GameObject.h"
#include "ModelComponent.h"
#include <string>

class POOT_API SkyBox : public GameObject3D
{
private:
	ModelComponent* model_comp;
	std::string cube_map_key;

public:
	SkyBox(const std::string cube_map_key);

protected:
	virtual void BuildComponents() override;
};

