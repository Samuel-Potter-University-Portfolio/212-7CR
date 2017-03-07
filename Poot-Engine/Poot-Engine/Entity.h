#pragma once
#include "API.h"
#include <vector>
#include "Transform.h"
#include "Component.h"
#include "Tickable.h"


typedef unsigned int Tags;

#define E_TAG_PLAYER	(1<<0)
#define E_TAG_PROP		(1<<1)
#define E_TAG_ENV		(1<<2)
#define E_TAG_SKYBOX	(1<<3)
#define E_TAG_UI		(1<<4)

#define E_TAG_LAST	E_TAG_UI

#define E_TAG_NONE 0
#define E_TAG_ALL (Tags)(-1)


class POOT_API Entity : public Tickable
{
private:
	std::vector<Component*> components;
	class World* world;
	bool is_visable = true;

protected:
	Tags tags = E_TAG_NONE;

	virtual void LogicBegin() override;
	virtual void WindowBegin() override;

	virtual void LogicTick(float delta_time) override;
	virtual void WindowTick(float delta_time) override;

public:
	Transform transform;

	virtual ~Entity();

	virtual void LogicDestroy();
	virtual void WindowDestroy();

	inline const bool IsVisable() { return is_visable && IsActive(); }
	inline void SetVisable(const bool visable) { is_visable = visable; }

	inline Tags GetTags() { return tags; }
	inline bool HasTag(Tags tag) { return tags | tag; }

	inline World* GetWorld() { return world; }
	inline void SetWorld(World* world) { this->world = world; }

	/* Components */
protected:
	void AddComponent(Component* component);

	template<typename CompType>
	inline CompType* MakeComponent()
	{
		CompType* new_comp = new CompType;
		AddComponent(new_comp);
		return new_comp;
	}

public:
	//Component fetching
	template<typename CompType>
	CompType* GetComponent() 
	{
		for (Component* comp : components)
		{
			CompType* actual_comp = dynamic_cast<CompType*>(comp);
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
			CompType* actual_comp = dynamic_cast<CompType*>(comp);
			if (actual_comp)
				actual_comps.push_back(actual_comp);
		}
		return actual_comps;
	}
};

