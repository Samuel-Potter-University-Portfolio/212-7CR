#pragma once
#include <Entity.h>
#include <ModelComponent.h>

class BasicBox : public Entity
{
private:
	ModelComponent* model;

public:
	BasicBox();

	virtual void WindowBegin() override;
};

