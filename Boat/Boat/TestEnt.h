#pragma once
#include "Entity.h"
#include "ModelComponent.h"

class TestEnt : public Entity
{
public:
	TestEnt();
	~TestEnt();
	ModelComponent* model_comp;
	Shader* shader;
	Model* model;

protected:
	//virtual void LogicBegin() override;
	virtual void WindowBegin() override;

	virtual void LogicTick(float delta_time) override;
	//virtual void WindowTick(float delta_time) override;

	//virtual void LogicDestroy() override;
	virtual void WindowDestroy() override;

};

