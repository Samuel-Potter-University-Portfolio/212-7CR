#pragma once
#include <vector>
#include "Component.h"
#include "Tickable.h"

class Entity : public Tickable
{
private:
	std::vector<Component*> components;
	bool is_visable = false;

protected:
	virtual void LogicBegin() override;
	virtual void WindowBegin() override;

	virtual void LogicTick(float delta_time) override;
	virtual void WindowTick(float delta_time) override;

public:
	~Entity();

	inline const bool IsVisable() { return is_visable; }
	inline void SetVisable(const bool visable) { is_visable = visable; }

protected:
	void AddComponent(Component* component);

public:
	//Component fetching
	template<typename CompType>
	CompType* GetComponent() 
	{
		for (Component* comp : components)
		{
			CompType* actual_comp = (CompType*)comp;
			if (actual_comp)
				return actual_comp;
		}
		return nullptr;
	}
	
	template<typename CompType>
	std::vector<CompType*> GetAllComponents()
	{
		std::vector<CompType*> actual_comps;

		for (Component* comp : components)
		{
			CompType* actual_comp = (CompType*)comp;
			if (actual_comp)
				actual_comps.push_back(actual_comp);
		}
		return actual_comps;
	}
};

