#pragma once
#include "Tickable.h"
#include "Transform.h"


class Component : public Tickable
{
private:
	bool active = true;

protected:
	class Entity* parent;

public:
	Transform transform;

	virtual void LogicDestroy() {};
	virtual void WindowDestroy() {};
	virtual void LogicTick(float delta_time) override;

	void SetParent(class Entity* parent) { this->parent = parent; }

	inline const bool IsActive() { return active; }
	inline void SetActive(const bool active) { this->active = active; }

	inline class Entity* GetParent() { return parent; }

	glm::mat4 GetTransformationMatrix(float lerp_factor);

	glm::vec3 GetWorldLocation();
	glm::vec3 GetWorldLocation(float lerp);
};

