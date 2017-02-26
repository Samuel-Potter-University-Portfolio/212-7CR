#pragma once
#include "Entity.h"
#include "ModelComponent.h"

enum CrossingLineType 
{
	Start,
	Finish
};


class CrossingLine : public Entity
{
public:
	CrossingLine(CrossingLineType type);

	const CrossingLineType type;
	ModelComponent* model_comp;

protected:
	//virtual void LogicBegin() override;
	virtual void WindowBegin() override;

	//virtual void LogicTick(float delta_time) override;
	//virtual void WindowTick(float delta_time) override;

	//virtual void LogicDestroy() override;
	//virtual void WindowDestroy() override;
};

