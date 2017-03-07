#pragma once
#include "Entity.h"
#include "ModelComponent.h"

enum BarrierDirection
{
	Horizontal,
	Vertical
};


class FloatBarrier : public Entity
{
public:
	FloatBarrier(BarrierDirection direction);
	ModelComponent* model_comp;

protected:
	//virtual void LogicBegin() override;
	virtual void WindowBegin() override;

	//virtual void LogicTick(float delta_time) override;
	//virtual void WindowTick(float delta_time) override;

	//virtual void LogicDestroy() override;
	//virtual void WindowDestroy() override;

};

