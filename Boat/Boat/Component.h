#pragma once
#include "Tickable.h"

class Component : public Tickable
{
private:
	bool active = true;
	class Entity* parent;

public:
	void SetParent(class Entity* parent) { this->parent = parent; }

	inline const bool IsActive() { return active; }
	inline void SetActive(const bool active) { this->active = active; }
};

