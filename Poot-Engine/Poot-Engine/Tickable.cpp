#include "Tickable.h"


Tickable::Tickable() 
	: active(true)
{
}

void Tickable::ForceLogicBegin() 
{
	LogicBegin();
	logic_begun = true;
}

void Tickable::ForceWindowBegin() 
{
	WindowBegin();
	window_begun = true;
}

void Tickable::HandleLogicTick(float delta_time) 
{
	if(active)
	{
		if (!logic_begun)
			ForceLogicBegin();
		else
			LogicTick(delta_time);
	}

}

void Tickable::HandleWindowTick(float delta_time)
{
	if (active)
	{
		if (!window_begun)
			ForceWindowBegin();
		else
			WindowTick(delta_time);
	}
}