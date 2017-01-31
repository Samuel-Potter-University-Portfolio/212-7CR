#pragma once
#include "Tickable.h"

class Component : public Tickable
{
private:
	bool active = true;
protected:
	class Entity* parent;

public:
	virtual void LogicDestroy() {};
	virtual void WindowDestroy() {};

	void SetParent(class Entity* parent) { this->parent = parent; }

	inline const bool IsActive() { return active; }
	inline void SetActive(const bool active) { this->active = active; }

	inline class Entity* GetParent() { return parent; }
};

